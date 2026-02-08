#include "CSVHandler.h"

#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

string intToStr(int value) {
    if (value == 0) return "0";
    string res = "";
    while (value > 0) {
        res = (char)((value % 10) + '0') + res;
        value /= 10;
    }
    return res;
}


static string trim(const string& s) {
    int start = 0;
    while (start < (int)s.length() && (s[start] == ' ' || s[start] == '\t')) start++;

    int end = (int)s.length() - 1;
    while (end >= 0 && (s[end] == ' ' || s[end] == '\t' || s[end] == '\r')) end--;

    if (end < start) return "";
    return s.substr(start, end - start + 1);
}


static string parseCSVField(stringstream& ss) {
    string field;
    char ch;
    

    while (ss.peek() == ' ' || ss.peek() == '\t') {
        ss.get();
    }
    

    if (ss.peek() == '"') {
        ss.get(); // consume opening quote
        while (ss.get(ch)) {
            if (ch == '"') {
                // Check for escaped quote (two quotes)
                if (ss.peek() == '"') {
                    ss.get(); // consume second quote
                    field += '"';
                } else {
                    
                    break;
                }
            } else {
                field += ch;
            }
        }
        // Consume trailing comma if present
        while (ss.peek() == ' ' || ss.peek() == '\t') {
            ss.get();
        }
        if (ss.peek() == ',') {
            ss.get();
        }
    } else {
        // Unquoted field - read until comma or end
        while (ss.get(ch)) {
            if (ch == ',') {
                break;
            }
            field += ch;
        }
        field = trim(field);
    }
    
    return field;
}

bool CSVHandler::loadGames(const string& filename, GameDynamicArray& games) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "[ERROR] Could not open games.csv" << endl;
        return false;
    }

    string line;
    // 1. Skip the header row (name, minplayers, etc.)
    getline(file, line);

    int idCounter = 1; // Start counting from 1 for G001, G002...

    while (getline(file, line)) {
        if (line.length() == 0) continue;

        // --- STEP 1: Generate the ID "Gxxx" manually ---
        string id = "G";
        if (idCounter < 100) id += "0";
        if (idCounter < 10) id += "0";
        id += intToStr(idCounter);
        idCounter++;

        // --- STEP 2: Initialize variables to 0 (Fixes "not defined" errors) ---
        string title = "";
        int minP = 0, maxP = 0, year = 0;

        // --- STEP 3: Manual Parsing (No stringstream) ---
        // This uses string::find and string::substr which are standard string methods
        size_t start = 0;
        size_t end = line.find(',');

        // Field 1: Name (Handle quotes if they exist)
        title = line.substr(start, end - start);
        if (title.length() > 0 && title[0] == '"') {
            title = title.substr(1, title.length() - 2);
        }

        // Field 2: minplayers
        start = end + 1;
        end = line.find(',', start);
        string minPStr = line.substr(start, end - start);

        // Field 3: maxplayers
        start = end + 1;
        end = line.find(',', start);
        string maxPStr = line.substr(start, end - start);

        // Fields 4 & 5: (Playtimes - we skip these)
        start = end + 1;
        end = line.find(',', start); // skip maxplaytime
        start = end + 1;
        end = line.find(',', start); // skip minplaytime

        // Field 6: yearpublished
        start = end + 1;
        string yearStr = line.substr(start);

        // --- STEP 4: Convert strings to integers manually ---
        // Using a basic loop to avoid stoi if necessary
        for (char c : minPStr) if (c >= '0' && c <= '9') minP = minP * 10 + (c - '0');
        for (char c : maxPStr) if (c >= '0' && c <= '9') maxP = maxP * 10 + (c - '0');
        for (char c : yearStr) if (c >= '0' && c <= '9') year = year * 10 + (c - '0');

        // --- STEP 5: Create Game object and add to your Dynamic Array ---
        // We set totalCopies and availableCopies to 1 by default
        Game g(id, title, minP, maxP, year, 1, 1);
        games.add(g);
    }

    file.close();
    return true;
}
bool CSVHandler::saveGames(const string& filename, GameDynamicArray& games) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "[ERROR] Unable to write to " << filename << "\n";
        return false;
    }

    // Header
    file << "name,minplayers,maxplayers,maxplaytime,minplaytime,yearpublished\n";

    for (int i = 0; i < games.size(); i++) {
        Game g = games.get(i);

        file << "\"" << g.getTitle() << "\","
            << g.getMinPlayers() << ","
            << g.getMaxPlayers() << ","
            << "0" << ","
            << "0" << ","
            << g.getYear() << "\n";
    }

    file.close();
    return true;
}

bool CSVHandler::loadUsers(const string& filename, UserDynamicArray& users) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "[ERROR] Unable to open " << filename << "\n";
        return false;
    }

    string line;
    // Skip header
    if (!getline(file, line)) {
        file.close();
        return false;
    }

    while (getline(file, line)) {
        if (trim(line) == "") continue;

        stringstream ss(line);

        string userID, name, roleStr;

        getline(ss, userID, ',');
        getline(ss, name, ',');
        getline(ss, roleStr, ',');

        userID = trim(userID);
        name = trim(name);
        roleStr = trim(roleStr);

        Role role = MEMBER;
        if (roleStr == "ADMIN") role = ADMIN;
        else role = MEMBER;

        User u(userID, name, role);
        users.add(u);
    }

    file.close();
    return true;
}

bool CSVHandler::saveUsers(const string& filename, UserDynamicArray& users) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "[ERROR] Unable to write to " << filename << "\n";
        return false;
    }

    file << "userID,name,role\n";

    for (int i = 0; i < users.size(); i++) {
        User u = users.get(i);

        file << u.getUserID() << ","
            << u.getName() << ","
            << (u.getRole() == ADMIN ? "ADMIN" : "MEMBER") << "\n";
    }

    file.close();
    return true;
}

bool CSVHandler::loadBorrowRecords(const string& filename, BorrowLinkedList& records) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "[ERROR] Unable to open " << filename << "\n";
        return false;
    }

    string line;
    // Skip header
    if (!getline(file, line)) {
        file.close();
        return false;
    }

    while (getline(file, line)) {
        if (trim(line) == "") continue;

        stringstream ss(line);

        string recordID, userID, gameID, borrowDate, returnDate;

        getline(ss, recordID, ',');
        getline(ss, userID, ',');
        getline(ss, gameID, ',');
        getline(ss, borrowDate, ',');
        getline(ss, returnDate, ',');

        recordID = trim(recordID);
        userID = trim(userID);
        gameID = trim(gameID);
        borrowDate = trim(borrowDate);
        returnDate = trim(returnDate);

        BorrowRecord br(recordID, userID, gameID, borrowDate, returnDate);
        records.insertFront(br);
    }

    file.close();
    return true;
}

bool CSVHandler::saveBorrowRecords(const string& filename, BorrowLinkedList& records) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "[ERROR] Unable to write to " << filename << "\n";
        return false;
    }

    file << "recordID,userID,gameID,borrowDate,returnDate\n";
    records.writeToCSV(file);

    file.close();
    return true;
}
bool CSVHandler::loadReviews(const string& filename, GameDynamicArray& games) {
    ifstream file(filename);
    if (!file.is_open()) {
        // Not an error; file might not exist yet on first run
        return false; 
    }

    string line;
    // Skip header: gameID,memberName,rating,comment
    if (!getline(file, line)) return false;

    while (getline(file, line)) {
        if (trim(line) == "") continue;

        stringstream ss(line);
        string gID, name, rateStr, comm;

        getline(ss, gID, ',');
        getline(ss, name, ',');
        getline(ss, rateStr, ',');
        getline(ss, comm, ','); // This assumes comments don't contain commas

        gID = trim(gID);
        name = trim(name);
        int rate = 0;
        for (char c : trim(rateStr)) if (c >= '0' && c <= '9') rate = rate * 10 + (c - '0');
        comm = trim(comm);

        // Find the game pointer and add the review to its BST
        for (int i = 0; i < games.size(); i++) {
            if (games.get(i).getGameID() == gID) {
                // We use getPtr to ensure we are modifying the actual game in the array
                games.getPtr(i)->addReview(name, comm, rate);
                break;
            }
        }
    }

    file.close();
    return true;
}

bool CSVHandler::saveReviews(const string& filename, GameDynamicArray& games) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "[ERROR] Unable to write reviews to " << filename << "\n";
        return false;
    }

    // Header row
    file << "gameID,memberName,rating,comment\n";

    for (int i = 0; i < games.size(); i++) {
        // This calls the Game::saveReviews method we wrote earlier, 
        // which performs the Pre-Order traversal of the BST.
        games.get(i).saveReviews(file);
    }

    file.close();
    return true;
}
