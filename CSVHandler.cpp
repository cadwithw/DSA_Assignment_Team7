#include "CSVHandler.h"

#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

// Helper: trim spaces (optional but useful)
static string trim(const string& s) {
    int start = 0;
    while (start < (int)s.length() && (s[start] == ' ' || s[start] == '\t')) start++;

    int end = (int)s.length() - 1;
    while (end >= 0 && (s[end] == ' ' || s[end] == '\t' || s[end] == '\r')) end--;

    if (end < start) return "";
    return s.substr(start, end - start + 1);
}

// Helper: parse CSV field handling quoted fields
static string parseCSVField(stringstream& ss) {
    string field;
    char ch;
    
    // Skip leading whitespace
    while (ss.peek() == ' ' || ss.peek() == '\t') {
        ss.get();
    }
    
    // Check if field is quoted
    if (ss.peek() == '"') {
        ss.get(); // consume opening quote
        while (ss.get(ch)) {
            if (ch == '"') {
                // Check for escaped quote (two quotes)
                if (ss.peek() == '"') {
                    ss.get(); // consume second quote
                    field += '"';
                } else {
                    // End of quoted field
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

        string name = parseCSVField(ss);
        string minPStr = parseCSVField(ss);
        string maxPStr = parseCSVField(ss);
        string maxPlaytimeStr = parseCSVField(ss);
        string minPlaytimeStr = parseCSVField(ss);
        string yearStr = parseCSVField(ss);

        // Validate and parse numeric fields with error handling
        if (name.empty()) continue; // Skip rows with empty name

        int minPlayers = 0;
        int maxPlayers = 0;
        int year = 0;

        try {
            minPlayers = stoi(trim(minPStr));
            maxPlayers = stoi(trim(maxPStr));
            year = stoi(trim(yearStr));
        } catch (const exception& e) {
            cout << "[WARNING] Skipping game '" << name << "' due to invalid numeric data: " << e.what() << "\n";
            continue;
        }

        // Default values for fields not in CSV
        int totalCopies = 1;
        int availableCopies = 1;

        Game g(name, name, minPlayers, maxPlayers, year, totalCopies, availableCopies);
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
