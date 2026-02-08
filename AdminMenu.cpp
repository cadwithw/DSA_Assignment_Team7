#include "AdminMenu.h"
#include "CSVHandler.h"
#include <iostream>

using namespace std;


 
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

static int getValidInt(string prompt, int minVal, int maxVal) {
    int val;
    while (true) {
        cout << prompt;
        // Check if input is a number AND within the specific range
        if (cin >> val && val >= minVal && val <= maxVal) {
            cin.ignore(1000, '\n');
            return val;
        }
        else {
            cout << "[INVALID] Please enter a number between " << minVal << " and " << maxVal << ".\n";
            cin.clear();           // Reset error flags
            cin.ignore(1000, '\n'); // Discard bad input
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

    int nextNum = games.size() + 1;
    string id = "G" + (nextNum < 100 ? string("0") : "") + (nextNum < 10 ? string("0") : "") + intToStr(nextNum);

    cout << "\nAdding New Game (Assigned ID: " << id << ")\n";
    cin.ignore(1000, '\n');

    while (true) {
        cout << "Title: ";
        getline(cin, title);
        if (title.length() >= 2) break; // Ensure title isn't empty or a single character
        cout << "[ERROR] Title must be at least 2 characters long.\n";
    }

    // Min Players: 1-20
    int minP = getValidInt("Min players (1-20): ", 1, 20);

    // Max Players: must be >= minP and <= 100
    int maxP = getValidInt("Max players (" + intToStr(minP) + "-100): ", minP, 100);

    // Year: 4 digits (1900 to current-ish)
    int year = getValidInt("Year published (1900-2026): ", 1900, 2026);

    // Copies: 1-100
    int total = getValidInt("Total copies to add (1-100): ", 1, 100);

    Game newGame(id, title, minP, maxP, year, total, total);
    games.add(newGame);
    CSVHandler::saveGames("games.csv", games); // Auto-save
    cout << "\n[SUCCESS] Game '" << title << "' added to inventory.\n";
}

/**
 * Logic for removing a game and updating the CSV file.
 */
void AdminMenu::handleRemoveGame(GameDynamicArray& games) {
    string id;
    cout << "Enter Game ID to remove (e.g., G001): ";
    cin >> id;

    // 1. First, find if the game actually exists
    Game* g = games.findByGameID(id);

    if (g != nullptr) {
        // 2. Display the game details so the user knows exactly what they are deleting
        cout << "\n[CONFIRMATION REQUIRED]" << endl;
        cout << "Are you sure you want to remove: " << g->getTitle() << " (" << id << ")?" << endl;

        char confirm;
        while (true) {
            cout << "Type 'Y' to confirm or 'N' to cancel: ";
            cin >> confirm;
            confirm = toupper(confirm); // Handle lowercase 'y' or 'n'

            if (confirm == 'Y') {
                // 3. Proceed with deletion
                if (games.removeByGameID(id)) {
                    CSVHandler::saveGames("games.csv", games);
                    cout << "[SUCCESS] Game " << id << " removed and CSV updated.\n";
                }
                break;
            }
            else if (confirm == 'N') {
                cout << "[CANCELLED] Deletion aborted. Game remains in inventory.\n";
                break;
            }
            else {
                cout << "[INVALID] Please enter Y or N.\n";
                cin.clear();
                cin.ignore(1000, '\n');
            }
        }
    }
    else {
        cout << "[ERROR] Game ID " << id << " not found.\n";
    }
}

 /**
  * Logic for adding new members with unique ID check.
  */
void AdminMenu::handleAddMember(UserDynamicArray& users) {
    string userID, name;

    while (true) {
        cout << "Enter new User ID (e.g., M101): ";
        cin >> userID;
        if (userID.length() >= 3) {
            if (users.findByUserID(userID) == nullptr) break;
            cout << "[ERROR] User ID already exists.\n";
        }
        else {
            cout << "[ERROR] ID must be at least 3 characters.\n";
        }
    }

    cin.ignore(1000, '\n');
    while (true) {
        cout << "Enter Name: ";
        getline(cin, name);
        if (name.length() >= 2) break;
        cout << "[ERROR] Name must be at least 2 characters.\n";
    }

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
    // Validate player count: must be between 1 and 100
    int players = getValidInt("Enter number of players to filter by (1-100): ", 1, 100);

    GameDynamicArray filtered;

    for (int i = 0; i < games.size(); i++) {
        Game g = games.get(i);
        if (players >= g.getMinPlayers() && players <= g.getMaxPlayers()) {
            filtered.add(g);
        }
    }

    if (filtered.size() == 0) {
        cout << "[INFO] No games found that support " << players << " players.\n";
        return;
    }

    cout << "\nSort results by:\n";
    cout << "1. Year Published\n";
    cout << "2. Max Player Count\n";
    cout << "0. Cancel (Don't sort)\n";

    // Validate sort choice: must be 0, 1, or 2
    int sortChoice = getValidInt("Choice: ", 0, 2);

    // If user picks 0, just show the table without sorting
    if (sortChoice != 0) {


  

        // Bubble Sort Algorithm
        for (int i = 0; i < filtered.size() - 1; i++) {
            for (int j = 0; j < filtered.size() - i - 1; j++) {
                bool swapNeeded = false;

                if (sortChoice == 1) {
                    if (filtered.get(j).getYear() > filtered.get(j + 1).getYear())
                        swapNeeded = true;
                }
                else if (sortChoice == 2) {
                    if (filtered.get(j).getMaxPlayers() > filtered.get(j + 1).getMaxPlayers())
                        swapNeeded = true;
                }

                if (swapNeeded) {
                    Game temp = filtered.get(j);
                    *filtered.getPtr(j) = filtered.get(j + 1);
                    *filtered.getPtr(j + 1) = temp;
                }
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