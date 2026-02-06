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

void AdminMenu::show(GameDynamicArray& games,
    UserDynamicArray& users,
    BorrowLinkedList& records) {
    int choice = -1;

    while (choice != 0) {
        cout << "\n=== Admin Menu ===\n";
        cout << "1. Add a new board game\n";
        cout << "2. Remove a board game\n";
        cout << "3. Add a new member\n";
        cout << "4. Display summary of games borrowed/returned\n";
        cout << "5. Display game details\n";
        cout << "6. List games by player count (sorted by year)\n";
        cout << "0. Exit\n";
        cout << "Choice: ";

        cin >> choice;

        switch (choice) {
        case 1: { // add a new board game
            string title = "";
            int minP = 0, maxP = 0, year = 0, total = 0;

            // auto generate id
            int nextNum = games.size() + 1;
            string id = "G";
            if (nextNum < 100) id += "0";
            if (nextNum < 10) id += "0";
            id += intToStr(nextNum);

            cout << "System assigned Game ID: " << id << endl;

            cin.ignore();

            // validate title 
            while (true) {
                cout << "Title: ";
                getline(cin, title);
                if (title.length() > 0) break;
                cout << "Error: Title cannot be empty.\n";
            }

            //validate number input
            auto getValidInt = [](string prompt) {
                int val;
                while (true) {
                    cout << prompt;
                    if (cin >> val && val >= 0) {
                        return val;
                    }
                    else {
                        cout << "Invalid input. Please enter a positive number.\n";
                        cin.clear(); 
                        cin.ignore(1000, '\n'); 
                    }
                }
                };
             
            minP = getValidInt("Min players: ");
            maxP = getValidInt("Max players: ");

            //validation
            while (maxP < minP) {
                cout << "Error: Max players cannot be less than Min players.\n";
                maxP = getValidInt("Max players: ");
            }

            year = getValidInt("Year: ");
            total = getValidInt("Total copies: ");

            Game newGame(id, title, minP, maxP, year, total, total);
            games.add(newGame);
            cout << "Game [ " << title << " ] added successfully!\n";
            break;
        }

        case 2: { // remove game
            string id;
            cout << "Enter Game ID to remove: ";
            cin >> id;

            if (games.removeByGameID(id)) {
                CSVHandler::saveGames("games.csv", games);
                cout << "Game removed successfully.\n";
            }
            else {
                cout << "Game not found.\n";
            }
            break;
        }

        case 3: { // add member
            string userID, name;

            cout << "User ID: ";
            cin >> userID;

            if (users.findByUserID(userID) != nullptr) {
                cout << "User ID already exists.\n";
                break;
            }

            cin.ignore();
            cout << "Name: ";
            getline(cin, name);

            User u(userID, name, MEMBER);
            users.add(u);

            CSVHandler::saveUsers("users.csv", users);
            cout << "Member added successfully.\n";
            break;
        }


        case 4: { // borrow/return summary
            cout << "\n--- Borrowing Records Summary ---\n";

            if (records.isEmpty()) {
                cout << "No borrowing records have been created yet.\n";
            }
            else {
                records.printAll();
            }

            cout << "---------------------------------\n";
            break;
        }

        case 5: { // game search (display particulars)
            string id;
            cout << "Enter Game ID: ";
            cin >> id;

            Game* g = games.findByGameID(id);
            if (g)
                g->print();
            else
                cout << "Game not found.\n";

            break;
        }

        case 6: { // list & sort games
            int players = 0;
            cout << "Enter number of players to filter by: ";
            cin >> players;

            GameDynamicArray filtered;

            //filter the games 
            for (int i = 0; i < games.size(); i++) {
                Game g = games.get(i);
                if (players >= g.getMinPlayers() && players <= g.getMaxPlayers()) {
                    filtered.add(g);
                }
            }

            if (filtered.size() == 0) {
                cout << "No games found for " << players << " players.\n";
                break;
            }

            // choose criteria 
            int sortChoice = 0;
            cout << "\nSort results by:\n";
            cout << "1. Year Published (Ascending)\n";
            cout << "2. Max Player Count (Ascending)\n";
            cout << "Choice: ";
            cin >> sortChoice;

            // do bubble sort
            for (int i = 0; i < filtered.size() - 1; i++) {
                for (int j = 0; j < filtered.size() - i - 1; j++) {
                    bool swapNeeded = false;

                    if (sortChoice == 1) { //sort by Year
                        if (filtered.get(j).getYear() > filtered.get(j + 1).getYear()) {
                            swapNeeded = true;
                        }
                    }
                    else { //sort by player count
                        if (filtered.get(j).getMaxPlayers() > filtered.get(j + 1).getMaxPlayers()) {
                            swapNeeded = true;
                        }
                    }

                    if (swapNeeded) {
                        Game temp = filtered.get(j);
                        *filtered.getPtr(j) = filtered.get(j + 1);
                        *filtered.getPtr(j + 1) = temp;
                    }
                }
            }

            cout << "\n--- Sorted Results ---\n";
            for (int i = 0; i < filtered.size(); i++) {
                filtered.get(i).print();
            }
            break;
        }

        case 0:
            cout << "Exiting Admin Menu...\n";
            break;

        default:
            cout << "Invalid choice.\n";
        }
    }
}
