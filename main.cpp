#include <iostream>
using namespace std;

#include "GameDynamicArray.h"
#include "UserDynamicArray.h"
#include "BorrowLinkedList.h"
#include "CSVHandler.h"

#include "AdminMenu.h"
#include "MemberMenu.h"
#include "AllUsersMenu.h"

int main() {
    GameDynamicArray games;
    UserDynamicArray users;
    BorrowLinkedList records;

    // Load CSV data
    CSVHandler::loadGames("games.csv", games);
    CSVHandler::loadUsers("users.csv", users);
    CSVHandler::loadBorrowRecords("borrow_records.csv", records);

    cout << "=========================================\n";
    cout << " NPTTGC Board Game Management Application \n";
    cout << "=========================================\n";

    while (true) {
        cout << "\nMain Menu\n";
        cout << "1. Login\n";
        cout << "0. Exit\n";
        cout << "Choice: ";

        int choice;
        cin >> choice;

        switch (choice) {
            case 0:
                goto exit_loop;
            case 1: {
                string userID;
                cout << "Enter User ID: ";
                cin >> userID;

                User* u = users.findByUserID(userID);

                if (u == nullptr) {
                    cout << "Invalid User ID.\n";
                    break;
                }

                if (u->isAdmin()) {
                    AdminMenu::show(games, users, records);
                } else {
                    MemberMenu::show(*u, games, records);
                    goto exit_loop;
                }
                break;
            }
            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
    }

    exit_loop:
    // Save before exit
    CSVHandler::saveGames("games.csv", games);
    CSVHandler::saveUsers("users.csv", users);
    CSVHandler::saveBorrowRecords("borrow_records.csv", records);

    cout << "Goodbye!\n";
    return 0;
}
