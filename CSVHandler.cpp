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

        string gameID, title, minPStr, maxPStr, yearStr, totalStr, availStr;

        getline(ss, gameID, ',');
        getline(ss, title, ',');
        getline(ss, minPStr, ',');
        getline(ss, maxPStr, ',');
        getline(ss, yearStr, ',');
        getline(ss, totalStr, ',');
        getline(ss, availStr, ',');

        gameID = trim(gameID);
        title = trim(title);

        int minPlayers = stoi(trim(minPStr));
        int maxPlayers = stoi(trim(maxPStr));
        int year = stoi(trim(yearStr));
        int totalCopies = stoi(trim(totalStr));
        int availableCopies = stoi(trim(availStr));

        Game g(gameID, title, minPlayers, maxPlayers, year, totalCopies, availableCopies);
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
    file << "gameID,title,minPlayers,maxPlayers,year,totalCopies,availableCopies\n";

    for (int i = 0; i < games.size(); i++) {
        Game g = games.get(i);

        file << g.getGameID() << ","
            << g.getTitle() << ","
            << g.getMinPlayers() << ","
            << g.getMaxPlayers() << ","
            << g.getYear() << ","
            << g.getTotalCopies() << ","
            << g.getAvailableCopies() << "\n";
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
