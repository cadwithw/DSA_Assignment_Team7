#include "MemberMenu.h"
#include "CSVHandler.h"
#include <iostream>
#include <ctime>
#include <sstream>

using namespace std;

// for borrowing game (to be recorded in borrow_records.csv)
static string generateRecordID(BorrowLinkedList& records) {
    // Simple ID generation: "BR" + counter
    static int counter = 1;
    return "BR" + to_string(counter++);
}

// for borrowing and returning game (to be recorded in borrow_records.csv)
static string getCurrentDate() {
    time_t now = time(0);
    tm timeinfo;
    localtime_s(&timeinfo, &now);
    
    stringstream ss;
    ss << (1900 + timeinfo.tm_year) << "-" 
       << (timeinfo.tm_mon + 1) << "-" 
       << timeinfo.tm_mday;
    
    return ss.str();
}

// for input validation (non integer inputs)
static int getValidChoice(int minChoice, int maxChoice, const string& prompt = "Choice: ") {
    int choice;
    while (true) {
        cout << prompt;
        if (cin >> choice && choice >= minChoice && choice <= maxChoice) {
            cin.ignore();
            return choice;
        } else {
            cout << "Invalid input. Please enter a number between " << minChoice << " and " << maxChoice << ".\n";
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }
}

// for input validation (non positive integer inputs)
static int getValidPositiveInt(const string& prompt) { 
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value && value > 0) {
            cin.ignore();
            return value;
        } else {
            cout << "Invalid input. Please enter a positive number.\n";
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }
}

// MEMBER MENU OPTIONS
// 1. Borrow a Game
static void borrowGame(const User& member, GameDynamicArray& games, BorrowLinkedList& records) {
    cout << "\n--- Borrow a Game ---\n";
    cout << "Search a game by:\n";
    cout << "1. Number of players\n";
    cout << "2. Year of Publication\n";
    cout << "3. Title\n";
    cout << "0. Back to Member Menu\n";

    int searchChoice = getValidChoice(0, 3);

    GameDynamicArray searchResults;
    
    switch (searchChoice) {
    case 0: {
        // Back to Member Menu
        cout << "Returning to Member Menu.\n";
        return;
    }
    case 1: {
        // Search by number of players
        int numPlayers = getValidPositiveInt("Enter number of players: ");

        for (int i = 0; i < games.size(); i++) {
            Game g = games.get(i);
            if (numPlayers >= g.getMinPlayers() && numPlayers <= g.getMaxPlayers()) {
                searchResults.add(g);
            }
        }

        if (searchResults.isEmpty()) {
            cout << "No games found for " << numPlayers << " players.\n";
            return;
        }
        break;
    }
    case 2: {
        // Search by year of publication
        int year = getValidPositiveInt("Enter year of publication: ");

        for (int i = 0; i < games.size(); i++) {
            Game g = games.get(i);
            if (g.getYear() == year) {
                searchResults.add(g);
            }
        }

        if (searchResults.isEmpty()) {
            cout << "No games found for year " << year << ".\n";
            return;
        }
        break;
    }
    case 3: {
        // Search by title (substring matching, case-insensitive)
        string searchTitle;
        cout << "Enter game title to search: ";
        getline(cin, searchTitle);

        if (searchTitle.empty()) {
            cout << "Title cannot be empty.\n";
            return;
        }

        // Convert search title to lowercase for case-insensitive comparison
        string searchTitleLower = searchTitle;
        for (int i = 0; i < (int)searchTitleLower.length(); i++) {
            searchTitleLower[i] = tolower(searchTitleLower[i]);
        }

        for (int i = 0; i < games.size(); i++) {
            Game g = games.get(i);
            string title = g.getTitle();
            
            // Convert game title to lowercase for comparison
            string titleLower = title;
            for (int j = 0; j < (int)titleLower.length(); j++) {
                titleLower[j] = tolower(titleLower[j]);
            }
            
            // Check if searchTitle is contained in the game title (case-insensitive)
            if (titleLower.find(searchTitleLower) != string::npos) {
                searchResults.add(g);
            }
        }

        if (searchResults.isEmpty()) {
            cout << "No games found containing '" << searchTitle << "'.\n";
            return;
        }
        break;
    }
    default:
        cout << "Invalid search choice.\n";
        return;
    }

    // Display search results
    cout << "\n--- Search Results ---\n";
    for (int i = 0; i < searchResults.size(); i++) {
        cout << "[" << (i + 1) << "] ";
        searchResults.get(i).print();
        cout << "-----------------------\n";
    }

    // Ask user to select a game
    int selection = getValidChoice(0, searchResults.size(), "Select a game to borrow (enter 0 to go back): ");

    if (selection == 0) {
        cout << "Returning to Borrow Menu.\n";
        return;
    }

    if (selection < 1 || selection > searchResults.size()) {
        cout << "Invalid selection.\n";
        return;
    }

    Game* foundGame = searchResults.getPtr(selection - 1);

    // Check if copies are available
    if (foundGame->getAvailableCopies() <= 0) {
        cout << "No copies of '" << foundGame->getTitle() << "' are currently available.\n";
        return;
    }

    // Display game details
    cout << "\nGame Details:\n";
    foundGame->print();

    // Confirm borrow
    cout << "\nConfirm borrow? (y/n): ";
    char confirm;
    while (true) {
        cin >> confirm;
        cin.ignore();
        if (confirm == 'y' || confirm == 'Y' || confirm == 'n' || confirm == 'N') {
            break;
        }
        cout << "Invalid input. Please enter 'y' or 'n': ";
    }

    if (confirm == 'y' || confirm == 'Y') {
        // Find the actual game in the main games array to update availability
        Game* actualGame = games.findByGameID(foundGame->getGameID());
        
        // Create borrow record
        string recordID = generateRecordID(records);      // BR1, BR2, BR3...
        string borrowDate = getCurrentDate();             // YYYY-M-D format
        string returnDate = "";                           // Empty (not returned)
        
        BorrowRecord newRecord(recordID, member.getUserID(), 
                           foundGame->getGameID(), borrowDate, returnDate);
        records.insertFront(newRecord);                   // Added to LinkedList

        // Decrease available copies in the main array
        if (actualGame != nullptr) {
            actualGame->decrementAvailable();
        }

        // Save the updated records to CSV 
        CSVHandler::saveBorrowRecords("borrow_records.csv", records);
        
        // Save the updated games to CSV (to save number of copies)
        CSVHandler::saveGames("games.csv", games);

        cout << "Game borrowed successfully!\n";
        cout << "Record ID: " << recordID << "\n";
        cout << "Borrow Date: " << borrowDate << "\n";
    } else {
        cout << "Borrow cancelled.\n";
    }
}

// 2. Return a Game
static void returnGame(const User& member, GameDynamicArray& games, BorrowLinkedList& records) {
    struct BorrowInfo {
        string recordID;
        string gameID;
        string borrowDate;
        Game* gamePtr;
    };

    BorrowInfo borrowedList[100];  // Assuming max 100 borrowed games
    int borrowedCount = 0;

    cout << "\nFetching your borrowed games...\n";
    
    cout << "\n--- Your Borrowed Games ---\n";
    
    int gameCount = 0;
    for (int i = 0; i < games.size(); i++) {
        Game g = games.get(i);
        BorrowRecord* activeBorrow = records.findActiveBorrow(member.getUserID(), g.getGameID());
        
        if (activeBorrow != nullptr) {
            gameCount++;
            cout << "[" << gameCount << "] " << g.getTitle() << "\n";
            cout << "    Game ID: " << g.getGameID() << "\n";
            cout << "    Borrow Date: " << activeBorrow->getBorrowDate() << "\n";
            cout << "-----------------------\n";
            
            // Store the borrow info
            if (borrowedCount < 100) {
                borrowedList[borrowedCount].recordID = activeBorrow->getRecordID();
                borrowedList[borrowedCount].gameID = g.getGameID();
                borrowedList[borrowedCount].borrowDate = activeBorrow->getBorrowDate();
                borrowedList[borrowedCount].gamePtr = games.findByGameID(g.getGameID());
                borrowedCount++;
            }
        }
    }

    if (borrowedCount == 0) {
        cout << "You have no borrowed games to return.\n";
        return;
    }

    // Ask user which game to return
    cout << "Select a game to return (enter 0 to go back): ";
    int selection = getValidChoice(0, borrowedCount);

    if (selection == 0) {
        cout << "Returning to Member Menu.\n";
        return;
    }

    if (selection < 1 || selection > borrowedCount) {
        cout << "Invalid selection.\n";
        return;
    }

    BorrowInfo& selectedBorrow = borrowedList[selection - 1];
    Game* gameToReturn = selectedBorrow.gamePtr;

    if (gameToReturn == nullptr) {
        cout << "Error: Game not found.\n";
        return;
    }

    // Display game details
    cout << "\nGame Details:\n";
    gameToReturn->print();

    // Confirm return
    cout << "\nConfirm return? (y/n): ";
    char confirm;
    while (true) {
        cin >> confirm;
        cin.ignore();
        if (confirm == 'y' || confirm == 'Y' || confirm == 'n' || confirm == 'N') {
            break;
        }
        cout << "Invalid input. Please enter 'y' or 'n': ";
    }

    if (confirm == 'y' || confirm == 'Y') {
        // Update the borrow record with return date
        BorrowRecord* borrowRecord = records.findActiveBorrow(member.getUserID(), selectedBorrow.gameID);
        if (borrowRecord != nullptr) {
            string returnDate = getCurrentDate();
            borrowRecord->setReturnDate(returnDate);

            // Increase available copies
            gameToReturn->incrementAvailable();

            // Save the updated records to CSV immediately
            CSVHandler::saveBorrowRecords("borrow_records.csv", records);
            
            // Save the updated games to CSV immediately (to persist availability changes)
            CSVHandler::saveGames("games.csv", games);

            cout << "Game returned successfully!\n";
            cout << "Record ID: " << selectedBorrow.recordID << "\n";
            cout << "Return Date: " << returnDate << "\n";
        } else {
            cout << "Error: Could not find borrow record.\n";
        }
    } else {
        cout << "Return cancelled.\n";
    }
}

// 3. Display Summary
static void displaySummary(const User& member, BorrowLinkedList& records) {
    cout << "\n--- Your Borrow Summary ---\n\n";

    string userID = member.getUserID();
    
    // Get statistics
    int totalBorrows = records.countBorrowsByUser(userID);
    int totalReturns = records.countReturnsByUser(userID);
    int activeBorrows = records.countActiveBorrowsByUser(userID);

    // Display user information
    cout << "User ID: " << member.getUserID() << "\n";
    cout << "Name: " << member.getName() << "\n";
    cout << "\n--- Statistics ---\n";
    cout << "Total Borrow Transactions: " << totalBorrows << "\n";
    cout << "Total Games Returned: " << totalReturns << "\n";
    cout << "Currently Borrowed: " << activeBorrows << "\n";
    
    if (totalBorrows > 0) {
        cout << "\nReturn Rate: " << ((totalReturns * 100) / totalBorrows) << "%\n";
    }

    cout << "\n--- Detailed History ---\n";
    records.printByUser(userID);
}

// 4. Review Games
static void reviewGames(GameDynamicArray& games) {
    cout << "\n--- Review Games ---\n";
    cout << "Review Games - Not yet implemented.\n";
}

// 5. Record a Game
static void recordGame(GameDynamicArray& games) {
    cout << "\n--- Record a Game ---\n";
    cout << "Record a Game - Not yet implemented.\n";
}

// Member Menu
void MemberMenu::show(const User& member, GameDynamicArray& games, BorrowLinkedList& records) {
    int choice = -1;

    while (choice != 0) {
        cout << "\n=== Member Menu ===\n";
        cout << "1. Borrow a Game\n";
        cout << "2. Return a Game\n";
        cout << "3. Display Summary\n";
        cout << "4. Review Games\n";
        cout << "5. Record a Game\n";
        cout << "6. Write a Review\n";
        cout << "0. Logout\n";

        choice = getValidChoice(0, 6);

        switch (choice) {
        case 1:
            borrowGame(member, games, records);
            break;
        case 2:
            returnGame(member, games, records);
            break;
        case 3:
            displaySummary(member, records);
            break;
        case 4:
            reviewGames(games);
            break;
        case 5:
            recordGame(games);
            break;
        case 6:
            
            break;
        case 0:
            cout << "Logging out...\n";
            cout << "Returning to Main Menu.\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
            break;
        }
    }
}

