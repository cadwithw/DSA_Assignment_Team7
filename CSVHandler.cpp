/******************************************************************************
 * Team Member: Ashton, Caden
 * Group: 7
 * Student IDs: S10267643, S10267163
 * Highlighted Features:
 * - Robust CSV Parsing: Manual implementation of field parsing to handle
 * quoted strings, escaped characters, and varied delimiters.
 * - Data Persistence: Full serialization and deserialization for Games,
 * Users, Borrow Records, and Reviews.
 * - Selective History Management: Capability to update individual user
 * browsing history while preserving global data.
 * - Manual Type Conversion: Implementation of custom string-to-int logic
 * to ensure compatibility across different compiler environments.
 *****************************************************************************/

#include "CSVHandler.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

/**
 * Converts an integer value to its string representation manually.
 * @param value The integer to be converted.
 * @return A string containing the numeric characters.
 */
string intToStr(int value) {
    if (value == 0) return "0";
    string res = "";
    while (value > 0) {
        res = (char)((value % 10) + '0') + res;
        value /= 10;
    }
    return res;
}

/**
 * Removes leading and trailing whitespace characters (spaces, tabs, carriage returns).
 * @param s The raw input string.
 * @return The cleaned string.
 */
static string trim(const string& s) {
    int start = 0;
    while (start < (int)s.length() && (s[start] == ' ' || s[start] == '\t')) start++;

    int end = (int)s.length() - 1;
    while (end >= 0 && (s[end] == ' ' || s[end] == '\t' || s[end] == '\r')) end--;

    if (end < start) return "";
    return s.substr(start, end - start + 1);
}

/**
 * Advanced CSV field extractor that handles quoted fields and escaped quotes.
 * @param ss The stringstream of the current CSV line.
 * @return The extracted and processed field string.
 */
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
                if (ss.peek() == '"') {
                    ss.get(); // consume second quote
                    field += '"';
                }
                else {
                    break;
                }
            }
            else {
                field += ch;
            }
        }
        while (ss.peek() == ' ' || ss.peek() == '\t') {
            ss.get();
        }
        if (ss.peek() == ',') {
            ss.get();
        }
    }
    else {
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

/**
 * Loads game data from a CSV file into the Dynamic Array.
 * Generates unique IDs (G001, etc.) during the import process.
 * @param filename Path to the games CSV.
 * @param games Reference to the storage array.
 * @return True if successful, false otherwise.
 */
bool CSVHandler::loadGames(const string& filename, GameDynamicArray& games) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "[ERROR] Could not open games.csv" << endl;
        return false;
    }

    string line;
    getline(file, line); // Skip header

    int idCounter = 1;

    while (getline(file, line)) {
        if (line.length() == 0) continue;

        string id = "G";
        if (idCounter < 100) id += "0";
        if (idCounter < 10) id += "0";
        id += intToStr(idCounter);
        idCounter++;

        string title = "";
        int minP = 0, maxP = 0, year = 0;

        size_t start = 0;
        size_t end = line.find(',');

        title = line.substr(start, end - start);
        if (title.length() > 0 && title[0] == '"') {
            title = title.substr(1, title.length() - 2);
        }

        start = end + 1;
        end = line.find(',', start);
        string minPStr = line.substr(start, end - start);

        start = end + 1;
        end = line.find(',', start);
        string maxPStr = line.substr(start, end - start);

        start = end + 1;
        end = line.find(',', start); // skip maxplaytime
        start = end + 1;
        end = line.find(',', start); // skip minplaytime

        start = end + 1;
        string yearStr = line.substr(start);

        for (char c : minPStr) if (c >= '0' && c <= '9') minP = minP * 10 + (c - '0');
        for (char c : maxPStr) if (c >= '0' && c <= '9') maxP = maxP * 10 + (c - '0');
        for (char c : yearStr) if (c >= '0' && c <= '9') year = year * 10 + (c - '0');

        Game g(id, title, minP, maxP, year, 1, 1);
        games.add(g);
    }

    file.close();
    return true;
}

/**
 * Saves the current game inventory back to the CSV file.
 * @param filename Path to the games CSV.
 * @param games Reference to the inventory array.
 * @return True if file was written successfully.
 */
bool CSVHandler::saveGames(const string& filename, GameDynamicArray& games) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "[ERROR] Unable to write to " << filename << "\n";
        return false;
    }

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

/**
 * Loads user/member accounts and their roles from a CSV file.
 * @param filename Path to the users CSV.
 * @param users Reference to the user directory array.
 * @return True if successful.
 */
bool CSVHandler::loadUsers(const string& filename, UserDynamicArray& users) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "[ERROR] Unable to open " << filename << "\n";
        return false;
    }

    string line;
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

        Role role = (roleStr == "ADMIN") ? ADMIN : MEMBER;

        User u(userID, name, role);
        users.add(u);
    }

    file.close();
    return true;
}

/**
 * Saves all user accounts and roles to the disk.
 * @param filename Path to the users CSV.
 * @param users Reference to the user directory array.
 * @return True if write successful.
 */
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

/**
 * Loads historical and active borrowing records into a Linked List.
 * @param filename Path to the records CSV.
 * @param records Reference to the Borrow Record Linked List.
 * @return True if successful.
 */
bool CSVHandler::loadBorrowRecords(const string& filename, BorrowLinkedList& records) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "[ERROR] Unable to open " << filename << "\n";
        return false;
    }

    string line;
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

        BorrowRecord br(trim(recordID), trim(userID), trim(gameID), trim(borrowDate), trim(returnDate));
        records.insertFront(br);
    }

    file.close();
    return true;
}

/**
 * Saves the entire linked list of borrow records to the CSV.
 * @param filename Path to the records CSV.
 * @param records Reference to the Linked List.
 * @return True if successful.
 */
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

/**
 * Loads reviews and attaches them to corresponding Games via BST insertion.
 * @param filename Path to reviews CSV.
 * @param games Array of games to match review IDs against.
 * @return True if file processed.
 */
bool CSVHandler::loadReviews(const string& filename, GameDynamicArray& games) {
    ifstream file(filename);
    if (!file.is_open()) return false;

    string line;
    if (!getline(file, line)) return false;

    while (getline(file, line)) {
        if (trim(line) == "") continue;
        stringstream ss(line);
        string gID, name, rateStr, comm;

        getline(ss, gID, ',');
        getline(ss, name, ',');
        getline(ss, rateStr, ',');
        getline(ss, comm);

        gID = trim(gID);
        name = trim(name);
        comm = trim(comm);

        int rate = 0;
        string tRate = trim(rateStr);
        for (char c : tRate) if (c >= '0' && c <= '9') rate = rate * 10 + (c - '0');

        for (int i = 0; i < games.size(); i++) {
            if (games.get(i).getGameID() == gID) {
                Game* targetGame = games.getPtr(i);
                if (targetGame != nullptr) {
                    targetGame->addReview(name, comm, rate);
                }
                break;
            }
        }
    }
    file.close();
    return true;
}

/**
 * Triggers recursive BST traversal for each game to save reviews to CSV.
 * @param filename Path to reviews CSV.
 * @param games Reference to games inventory.
 * @return True if successful.
 */
bool CSVHandler::saveReviews(const string& filename, GameDynamicArray& games) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "[ERROR] Unable to write reviews to " << filename << "\n";
        return false;
    }

    file << "gameID,memberName,rating,comment\n";
    for (int i = 0; i < games.size(); i++) {
        games.get(i).saveReviews(file);
    }

    file.close();
    return true;
}

/**
 * Loads the browsing history queue for a specific user ID.
 * @param filename Path to history CSV.
 * @param userID The ID of the currently logged-in user.
 * @param history The queue to populate.
 * @param games Inventory to link history back to Game objects.
 * @return True if file read.
 */
bool CSVHandler::loadBrowseHistory(const string& filename, const string& userID,
    BrowseHistory& history, GameDynamicArray& games) {
    ifstream file(filename);
    if (!file.is_open()) return false;

    string line;
    if (!getline(file, line)) {
        file.close();
        return false;
    }

    while (getline(file, line)) {
        if (trim(line) == "") continue;
        stringstream ss(line);
        string loadedUserID, gameID, viewedDate;

        getline(ss, loadedUserID, ',');
        getline(ss, gameID, ',');
        getline(ss, viewedDate, ',');

        if (trim(loadedUserID) == userID) {
            Game* game = games.findByGameID(trim(gameID));
            if (game != nullptr) {
                history.enqueue(*game, trim(viewedDate));
            }
        }
    }

    file.close();
    return true;
}

/**
 * Saves browsing history while ensuring other users' data is not overwritten.
 * @param filename Path to history CSV.
 * @param userID The ID of the current user.
 * @param history The history queue to save.
 * @return True if file updated.
 */
bool CSVHandler::saveBrowseHistory(const string& filename, const string& userID,
    BrowseHistory& history) {
    ifstream inFile(filename);
    vector<string> allLines;

    if (inFile.is_open()) {
        string line;
        if (getline(inFile, line)) {
            allLines.push_back(line);
        }

        while (getline(inFile, line)) {
            if (trim(line) == "") continue;
            stringstream ss(line);
            string lineUserID;
            getline(ss, lineUserID, ',');
            if (trim(lineUserID) != userID) {
                allLines.push_back(line);
            }
        }
        inFile.close();
    }

    ofstream outFile(filename);
    if (!outFile.is_open()) return false;

    if (allLines.empty()) {
        outFile << "userID,gameID,viewedDate\n";
    }
    else {
        outFile << allLines[0] << "\n";
    }

    for (int i = 1; i < (int)allLines.size(); i++) {
        outFile << allLines[i] << "\n";
    }

    int historySize = history.getSize();
    for (int i = 0; i < historySize; i++) {
        Game* game = history.getGameAt(i);
        if (game != nullptr) {
            outFile << userID << "," << game->getGameID() << ","
                << history.getViewedDateAt(i) << "\n";
        }
    }

    outFile.close();
    return true;
}