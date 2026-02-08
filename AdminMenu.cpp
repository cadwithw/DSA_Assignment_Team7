/******************************************************************************
 * Team Member: Ashton, Caden
 * Group: 7
 * Student IDs: S10267643, S10267163
 * Highlighted Features:
 * - Sorting: Implementation of the Bubble Sort algorithm to organize filtered
 * game lists by year or player count.
 * - Input Validation: Robust error handling for integer ranges and string lengths.
 * - Table Rendering: Custom-aligned console output for professional reporting.
 * - Data Integrity: Automatic CSV synchronization after modification (Add/Remove).
 *****************************************************************************/

#include "AdminMenu.h"
#include "CSVHandler.h"
#include <iostream>

using namespace std;

/**
 * Helper function to convert an integer to a string without using std::to_string.
 * @param value The integer to convert.
 * @return The string representation of the integer.
 */
static string intToStr(int value) {
    if (value == 0) return "0";
    string res = "";
    while (value > 0) {
        res = (char)((value % 10) + '0') + res;
        value /= 10;
    }
    return res;
}

/**
 * Utility to print a string with a fixed trailing width for table alignment.
 * @param text The string to display.
 * @param width The total width of the column.
 */
static void printColumn(string text, int width) {
    cout << text;
    for (int i = text.length(); i < width; i++) {
        cout << " ";
    }
}

/**
 * Forces the user to provide a valid integer within a specified range.
 * Clears the buffer and handles non-numeric input gracefully.
 * @param prompt The message shown to the user.
 * @param minVal Minimum allowed value.
 * @param maxVal Maximum allowed value.
 * @return A validated integer.
 */
static int getValidInt(string prompt, int minVal, int maxVal) {
    int val;
    while (true) {
        cout << prompt;
        if (cin >> val && val >= minVal && val <= maxVal) {
            cin.ignore(1000, '\n');
            return val;
        }
        else {
            cout << "[INVALID] Please enter a number between " << minVal << " and " << maxVal << ".\n";
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }
}

// --- AdminMenu Implementation ---

/**
 * Main entry point for the Admin interface.
 * Displays the menu and delegates tasks based on user choice.
 * @param games Reference to the game inventory array.
 * @param users Reference to the user directory array.
 * @param records Reference to the borrowing record linked list.
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
 * Handles the logic for adding a new game to the system.
 * Includes automated ID generation and field validation.
 * @param games Reference to the inventory to update.
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
        if (title.length() >= 2) break;
        cout << "[ERROR] Title must be at least 2 characters long.\n";
    }

    int minP = getValidInt("Min players (1-20): ", 1, 20);
    int maxP = getValidInt("Max players (" + intToStr(minP) + "-100): ", minP, 100);
    int year = getValidInt("Year published (1900-2026): ", 1900, 2026);
    int total = getValidInt("Total copies to add (1-100): ", 1, 100);

    Game newGame(id, title, minP, maxP, year, total, total);
    games.add(newGame);
    CSVHandler::saveGames("games.csv", games);
    cout << "\n[SUCCESS] Game '" << title << "' added to inventory.\n";
}

/**
 * Removes a game from the inventory after user confirmation.
 * @param games Reference to the inventory to update.
 */
void AdminMenu::handleRemoveGame(GameDynamicArray& games) {
    string id;
    cout << "Enter Game ID to remove (e.g., G001): ";
    cin >> id;

    Game* g = games.findByGameID(id);

    if (g != nullptr) {
        cout << "\n[CONFIRMATION REQUIRED]" << endl;
        cout << "Are you sure you want to remove: " << g->getTitle() << " (" << id << ")?" << endl;

        char confirm;
        while (true) {
            cout << "Type 'Y' to confirm or 'N' to cancel: ";
            cin >> confirm;
            confirm = toupper(confirm);

            if (confirm == 'Y') {
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
 * @param users Reference to the user collection.
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
 * Searches for a game by ID and prints detailed information including reviews.
 * @param games Reference to the inventory.
 */
void AdminMenu::handleGameSearch(GameDynamicArray& games) {
    string id;
    cout << "Enter Game ID to view: ";
    cin >> id;

    Game* g = games.findByGameID(id);
    if (g) {
        g->print();
    }
    else {
        cout << "[ERROR] Game not found.\n";
    }
}

/**
 * Filters games based on player count and allows sorting of the result.
 * Implements a Bubble Sort for organizing the filtered data.
 * @param games Reference to the inventory.
 */
void AdminMenu::handleFilterAndSort(GameDynamicArray& games) {
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

    int sortChoice = getValidInt("Choice: ", 0, 2);

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
 * Formats and renders a list of games into a tabular console view.
 * @param list The dynamic array of games to display.
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