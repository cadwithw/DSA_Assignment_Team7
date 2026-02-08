#include "AdminMenu.h"
#include "CSVHandler.h"
#include <iostream>

using namespace std;

// --- Private Utilities (Internal to this file) ---

static string intToStr(int value) {
    if (value == 0) return "0";
    string res = "";
    while (value > 0) {
        res = (char)((value % 10) + '0') + res;
        value /= 10;
    }
    return res;
}

static void printColumn(string text, int width) {
    cout << text;
    for (int i = text.length(); i < width; i++) {
        cout << " ";
    }
}

static int getValidInt(string prompt) {
    int val;
    while (true) {
        cout << prompt;
        if (cin >> val && val >= 0) {
            cin.ignore(1000, '\n'); // Clear buffer
            return val;
        }
        else {
            cout << "[INVALID] Please enter a positive whole number.\n";
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }
}

// --- AdminMenu Implementation ---

/**
 * Main entry point for the Admin interface.
 * Acts as a controller, delegating tasks to specific handler functions.
 */
void AdminMenu::show(GameDynamicArray& games, UserDynamicArray& users, BorrowLinkedList& records) {
    int choice = -1;

    while (choice != 0) {
        cout << "\n==============================\n";
        cout << "         ADMIN MENU           \n";
        cout << "==============================\n";
        cout << "1. Add a new board game\n";
        cout << "2. Remove a board game\n";
        cout << "3. Add a new member\n";
        cout << "4. Display borrowing summary\n";
        cout << "5. Display game details\n";
        cout << "6. List games by player count\n";
        cout << "0. Exit\n";
        cout << "------------------------------\n";
        cout << "Choice: ";

        if (!(cin >> choice)) {
            cout << "[INVALID] Please enter a number.\n";
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        switch (choice) {
        case 1: handleAddGame(games); break;
        case 2: handleRemoveGame(games); break;
        case 3: handleAddMember(users); break;
        case 4: {
            cout << "\n--- Borrowing Records Summary ---\n";
            if (records.isEmpty()) {
                cout << "No records found.\n";
            }
            else {
                records.printAll();
            }
            break;
        }
        case 5: handleGameSearch(games); break;
        case 6: handleFilterAndSort(games); break;
        case 0: cout << "Exiting Admin Menu...\n"; break;
        default: cout << "Invalid choice. Try again.\n";
        }
    }
}

/**
 * Logic for adding a new game, including ID generation and validation.
 */
void AdminMenu::handleAddGame(GameDynamicArray& games) {
    string title;

    // Auto-generate ID (G001, G002...)
    int nextNum = games.size() + 1;
    string id = "G";
    if (nextNum < 100) id += "0";
    if (nextNum < 10) id += "0";
    id += intToStr(nextNum);

    cout << "\nAdding New Game (Assigned ID: " << id << ")\n";
    cin.ignore();

    while (true) {
        cout << "Title: ";
        getline(cin, title);
        if (title.length() > 0) break;
        cout << "[ERROR] Title cannot be empty.\n";
    }

    int minP = getValidInt("Min players: ");
    int maxP = getValidInt("Max players: ");
    while (maxP < minP) {
        cout << "[ERROR] Max players cannot be less than Min players.\n";
        maxP = getValidInt("Max players: ");
    }

    int year = getValidInt("Year published: ");
    int total = getValidInt("Total copies to add: ");

    Game newGame(id, title, minP, maxP, year, total, total);
    games.add(newGame);
    cout << "\n[SUCCESS] Game '" << title << "' added to inventory.\n";
}

/**
 * Logic for removing a game and updating the CSV file.
 */
void AdminMenu::handleRemoveGame(GameDynamicArray& games) {
    string id;
    cout << "Enter Game ID to remove (e.g., G001): ";
    cin >> id;

    if (games.removeByGameID(id)) {
        CSVHandler::saveGames("games.csv", games);
        cout << "[SUCCESS] Game " << id << " removed and CSV updated.\n";
    }
    else {
        cout << "[ERROR] Game not found.\n";
    }
}

/**
 * Logic for adding new members with unique ID check.
 */
 /**
  * Logic for adding new members with unique ID check.
  */
void AdminMenu::handleAddMember(UserDynamicArray& users) {
    string userID, name;
    cout << "Enter new User ID: ";
    cin >> userID;

    // Check if user already exists
    if (users.findByUserID(userID) != nullptr) {
        cout << "[ERROR] User ID already exists.\n";
        return;
    }


    cin.ignore(1000, '\n'); // Clean buffer before getline
    cout << "Enter Name: ";
    getline(cin, name);

    User u(userID, name, MEMBER);
    users.add(u);
    CSVHandler::saveUsers("users.csv", users);
    cout << "[SUCCESS] Member '" << name << "' added.\n";
}

/**
 * Logic for searching and displaying specific game details (including reviews).
 */
void AdminMenu::handleGameSearch(GameDynamicArray& games) {
    string id;
    cout << "Enter Game ID to view: ";
    cin >> id;

    Game* g = games.findByGameID(id);
    if (g) {
        g->print(); // This will also trigger the BST review display
    }
    else {
        cout << "[ERROR] Game not found.\n";
    }
}

/**
 * Logic for filtering games by player count and sorting using Bubble Sort.
 */
void AdminMenu::handleFilterAndSort(GameDynamicArray& games) {
    int players = getValidInt("Enter number of players to filter by: ");
    GameDynamicArray filtered;

    for (int i = 0; i < games.size(); i++) {
        Game g = games.get(i);
        if (players >= g.getMinPlayers() && players <= g.getMaxPlayers()) {
            filtered.add(g);
        }
    }

    if (filtered.size() == 0) {
        cout << "No games found for " << players << " players.\n";
        return;
    }

    cout << "\nSort results by:\n1. Year Published\n2. Max Player Count\nChoice: ";
    int sortChoice;
    cin >> sortChoice;

    // Bubble Sort Algorithm
    for (int i = 0; i < filtered.size() - 1; i++) {
        for (int j = 0; j < filtered.size() - i - 1; j++) {
            bool swapNeeded = false;
            if (sortChoice == 1) {
                if (filtered.get(j).getYear() > filtered.get(j + 1).getYear()) swapNeeded = true;
            }
            else {
                if (filtered.get(j).getMaxPlayers() > filtered.get(j + 1).getMaxPlayers()) swapNeeded = true;
            }

            if (swapNeeded) {
                Game temp = filtered.get(j);
                *filtered.getPtr(j) = filtered.get(j + 1);
                *filtered.getPtr(j + 1) = temp;
            }
        }
    }

    displayGamesTable(filtered);
}

/**
 * Utility for rendering a professional table view of games.
 */
void AdminMenu::displayGamesTable(GameDynamicArray& list) {
    cout << "\n" << string(75, '=') << endl;
    printColumn("ID", 8);
    printColumn("Title", 35);
    printColumn("Players", 12);
    printColumn("Year", 8);
    cout << "Stock" << endl;
    cout << string(75, '-') << endl;

    for (int i = 0; i < list.size(); i++) {
        Game g = list.get(i);
        printColumn(g.getGameID(), 8);

        string displayTitle = g.getTitle();
        if (displayTitle.length() > 32) displayTitle = displayTitle.substr(0, 29) + "...";
        printColumn(displayTitle, 35);

        string playerRange = intToStr(g.getMinPlayers()) + "-" + intToStr(g.getMaxPlayers());
        printColumn(playerRange, 12);
        printColumn(intToStr(g.getYear()), 8);

        cout << g.getAvailableCopies() << " / " << g.getTotalCopies() << endl;
    }
    cout << string(75, '=') << endl;
}