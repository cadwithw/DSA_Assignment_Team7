#include <iostream>
#include <string>
using namespace std;

#include "GameDynamicArray.h"
#include "UserDynamicArray.h"
#include "BorrowLinkedList.h"
#include "CSVHandler.h"

#include "AdminMenu.h"
#include "MemberMenu.h"

// Validation for menu option (such as inputting strings)
static int getValidMenuChoice(int minChoice, int maxChoice) {
    int choice;
    while (true) {
        cout << "Choice: ";
        if (cin >> choice && choice >= minChoice && choice <= maxChoice) {
            cin.ignore();
            return choice;
        }
        else {
            cout << "Invalid input. Please enter a number between " << minChoice << " and " << maxChoice << ".\n";
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }
}

// Validation for User ID input
static string getValidUserID() {
    string userID;
    while (true) {
        cout << "Enter User ID: ";
        cin >> userID;
        cin.ignore();

        if (userID.empty()) {
            cout << "User ID cannot be empty. Please try again.\n";
            continue;
        }

        if (userID.length() > 20) {
            cout << "User ID is too long. Maximum 20 characters allowed.\n";
            continue;
        }

        return userID;
    }
}

int main() {
    GameDynamicArray games;
    UserDynamicArray users;
    BorrowLinkedList records;

    // --- STEP 1: Load CSV data ---
    if (!CSVHandler::loadGames("games.csv", games)) {
        cout << "[WARNING] Could not load games.csv. Continuing with empty game library.\n";
    }

    // NEW: Load Reviews immediately after games are loaded
    if (!CSVHandler::loadReviews("reviews.csv", games)) {
        cout << "[INFO] No reviews.csv found or file is empty.\n";
    }

    if (!CSVHandler::loadUsers("users.csv", users)) {
        cout << "[WARNING] Could not load users.csv. Continuing with no users.\n";
    }

    if (!CSVHandler::loadBorrowRecords("borrow_records.csv", records)) {
        cout << "[WARNING] Could not load borrow_records.csv. Continuing with no records.\n";
    }

    // --- STEP 2: Logic for Game Availability ---
    for (int i = 0; i < games.size(); i++) {
        Game* game = games.getPtr(i);
        string gameID = game->getGameID();

        int activeBorrowCount = 0;
        for (int userIdx = 0; userIdx < users.size(); userIdx++) {
            User* user = users.getPtr(userIdx);
            BorrowRecord* activeBorrow = records.findActiveBorrow(user->getUserID(), gameID);
            if (activeBorrow != nullptr) {
                activeBorrowCount++;
            }
        }

        for (int j = 0; j < activeBorrowCount; j++) {
            game->decrementAvailable();
        }
    }

    cout << "=========================================\n";
    cout << " NPTTGC Board Game Management Application \n";
    cout << "=========================================\n";

    bool exitProgram = false;

    while (!exitProgram) {
        cout << "\nMain Menu\n";
        cout << "1. Login\n";
        cout << "0. Exit\n";

        int choice = getValidMenuChoice(0, 1);

        switch (choice) {
        case 0: {
            cout << "Exiting application...\n";
            exitProgram = true;
            break;
        }
        case 1: {
            string userID = getValidUserID();
            User* u = users.findByUserID(userID);

            if (u == nullptr) {
                cout << "[ERROR] Invalid User ID. No user found with ID: " << userID << "\n";
                break;
            }

            cout << "Login successful.\nWelcome, " << u->getName() << "!\n";

            if (u->isAdmin()) {
                AdminMenu::show(games, users, records);
            }
            else {
                MemberMenu::show(*u, games, records);
            }
            break;
        }
        default:
            cout << "An unexpected error occurred. Please try again.\n";
            break;
        }
    }

    // --- STEP 3: Saving data before exit ---
    cout << "\nSaving data...\n";

    if (!CSVHandler::saveGames("games.csv", games)) {
        cout << "[ERROR] Failed to save games.csv\n";
    }
    else {
        cout << "[OK] Games saved successfully.\n";
    }

    // NEW: Save Reviews BST
    if (!CSVHandler::saveReviews("reviews.csv", games)) {
        cout << "[ERROR] Failed to save reviews.csv\n";
    }
    else {
        cout << "[OK] Reviews saved successfully.\n";
    }

    if (!CSVHandler::saveUsers("users.csv", users)) {
        cout << "[ERROR] Failed to save users.csv\n";
    }
    else {
        cout << "[OK] Users saved successfully.\n";
    }

    if (!CSVHandler::saveBorrowRecords("borrow_records.csv", records)) {
        cout << "[ERROR] Failed to save borrow_records.csv\n";
    }
    else {
        cout << "[OK] Borrow records saved successfully.\n";
    }

    cout << "Goodbye!\n";
    return 0;
}