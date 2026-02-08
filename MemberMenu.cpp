/******************************************************************************
 * Team Member: Ashton, Caden
 * Group: 7
 * Student IDs: S10267643, S10267163
 * Highlighted Features:
 * - Queue-Based Browse History: Maintains only the 5 most recent game views
 * with automatic removal of oldest entries for memory efficiency.
 * - Multi-Criteria Search: Supports searching games by player count, year,
 * and title with case-insensitive matching for flexibility.
 * - Real-Time Availability: Checks current stock when displaying browse history
 * and search results to prevent borrowing unavailable games.
 * - Transaction Persistence: Immediately saves borrow/return records and browse
 * history to CSV for data durability and audit trails.
 *****************************************************************************/

#include "MemberMenu.h"
#include "CSVHandler.h"
#include "BrowseHistory.h"
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
static void borrowGame(const User& member, GameDynamicArray& games, BorrowLinkedList& records, BrowseHistory& history) {
    bool continueSearching = true;
    
    while (continueSearching) {
        cout << "\n--- Borrow a Game ---\n";
        cout << "Search a game by:\n";
        cout << "1. Number of players\n";
        cout << "2. Year of Publication\n";
        cout << "3. Title\n";
        cout << "4. Recent Browse History\n";
        cout << "0. Back to Member Menu\n";

        int searchChoice = getValidChoice(0, 4);

        GameDynamicArray searchResults;
        
        switch (searchChoice) {
        case 0: {
            // Back to Member Menu
            cout << "Returning to Member Menu.\n";
            continueSearching = false;
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
                continue;
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
                continue;
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
                continue;
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
                continue;
            }
            break;
        }
        case 4: {
            // Search by recent browse history
            if (history.isEmpty()) {
                cout << "You haven't viewed any games yet.\n";
                continue;
            }

            cout << "\n--- Your Recent Browse History ---\n";
            int historyCount = history.getSize();
            
            // Display browse history with current stock from games array
            int displayCount = 1;
            for (int i = 0; i < historyCount && i < 5; i++) {
                string gameID = history.getGameIDAt(i);
                Game* histGame = history.getGameAt(i);
                
                if (histGame != nullptr) {
                    // Fetch current game data to get updated stock
                    Game* currentGame = nullptr;
                    for (int j = 0; j < games.size(); j++) {
                        if (games.get(j).getGameID() == gameID) {
                            currentGame = &(games.getPtr(j)[0]);
                            break;
                        }
                    }
                    
                    int availableCopies = (currentGame != nullptr) ? currentGame->getAvailableCopies() : histGame->getAvailableCopies();
                    int totalCopies = (currentGame != nullptr) ? currentGame->getTotalCopies() : histGame->getTotalCopies();
                    
                    cout << "[" << displayCount << "] " << histGame->getTitle() << "\n";
                    cout << "    Game ID: " << gameID << "\n";
                    cout << "    Players: " << histGame->getMinPlayers() << "-" 
                         << histGame->getMaxPlayers() << "\n";
                    cout << "    Stock: " << availableCopies << "/" << totalCopies << "\n";
                    cout << "-----------------------\n";
                    displayCount++;
                }
            }

            // Ask user to select from history
            int selection = getValidChoice(0, (displayCount - 1), "Select a game from history (enter 0 to go back): ");

            if (selection == 0) {
                cout << "Returning to Borrow Menu.\n";
                continue;
            }

            // Get the selected game from history
            Game* foundGame = history.getGameAt(selection - 1);
            
            if (foundGame == nullptr) {
                cout << "Error: Game not found in history.\n";
                continue;
            }

            // Find the actual game in the main games array to check availability
            Game* actualGame = games.findByGameID(foundGame->getGameID());
            
            if (actualGame == nullptr) {
                cout << "Error: Game not found in library.\n";
                continue;
            }

            // Check if copies are available
            if (actualGame->getAvailableCopies() <= 0) {
                cout << "No copies of '" << actualGame->getTitle() << "' are currently available.\n";
                continue;
            }

            // Display game details
            cout << "\nGame Details:\n";
            actualGame->print();

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
                // Create borrow record
                string recordID = generateRecordID(records);
                string borrowDate = getCurrentDate();
                string returnDate = "";
                
                BorrowRecord newRecord(recordID, member.getUserID(), 
                                   actualGame->getGameID(), borrowDate, returnDate);
                records.insertFront(newRecord);

                // Decrease available copies
                actualGame->decrementAvailable();

                // Save the updated records to CSV 
                CSVHandler::saveBorrowRecords("borrow_records.csv", records);
                
                // Save the updated games to CSV
                CSVHandler::saveGames("games.csv", games);
                
                // Add to browse history and save immediately
                history.enqueue(*actualGame, getCurrentDate());
                CSVHandler::saveBrowseHistory("browse_history.csv", member.getUserID(), history);

                cout << "Game borrowed successfully!\n";
                cout << "Record ID: " << recordID << "\n";
                cout << "Borrow Date: " << borrowDate << "\n";
            } else {
                cout << "Borrow cancelled.\n";
            }
            continue;
        }

        default:
            cout << "Invalid search choice.\n";
            continue;
        }

        // Display search results
        cout << "\n--- Search Results ---\n";
        for (int i = 0; i < searchResults.size(); i++) {
            cout << "[" << (i + 1) << "] ";
            searchResults.get(i).printWithoutReviews();
            cout << "-----------------------\n";
        }

        // Ask user to select a game
        int selection = getValidChoice(0, searchResults.size(), "Select a game to borrow (enter 0 to go back): ");

        if (selection == 0) {
            cout << "Returning to Borrow Menu.\n";
            continue;
        }

        if (selection < 1 || selection > searchResults.size()) {
            cout << "Invalid selection.\n";
            continue;
        }

        Game* foundGame = searchResults.getPtr(selection - 1);

        // Check if copies are available
        if (foundGame->getAvailableCopies() <= 0) {
            cout << "No copies of '" << foundGame->getTitle() << "' are currently available.\n";
            continue;
        }

        // ADD TO BROWSE HISTORY (Queue - regardless of search method)
        history.enqueue(*foundGame, getCurrentDate());
        
        // Save browse history immediately to ensure persistence
        CSVHandler::saveBrowseHistory("browse_history.csv", member.getUserID(), history);

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
}

// 2. Return a Game
static void returnGame(const User& member, GameDynamicArray& games, BorrowLinkedList& records, BrowseHistory& history) {
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
static void displaySummary(const User& member, GameDynamicArray& games, BorrowLinkedList& records, BrowseHistory& history) {
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
    records.printByUser(userID, &games);
}

// ADDITIONAL FEATURES
// View Browse History (Queue - FIFO)
static void viewBrowseHistory(BrowseHistory& history, GameDynamicArray& games) {
    cout << "\n--- Browse History ---\n";
    if (history.isEmpty()) {
        cout << "You haven't viewed any games yet.\n";
        return;
    }
    
    history.displayHistory(&games);
}

// Member Menu
void MemberMenu::show(const User& member, GameDynamicArray& games, BorrowLinkedList& records) {
    // Create and load browse history for this user
    BrowseHistory history;
    CSVHandler::loadBrowseHistory("browse_history.csv", member.getUserID(), history, games);
    
    int choice = -1;

    while (choice != 0) {
        cout << "\n=== Member Menu ===\n";
        cout << "1. Borrow a Game\n";
        cout << "2. Return a Game\n";
        cout << "3. Display Summary\n";
        cout << "4. Review Games\n";
        cout << "5. View Review \n";
        cout << "0. Logout\n";

        choice = getValidChoice(0, 6);

        switch (choice) {
        case 1:
            borrowGame(member, games, records, history);
            break;
        case 2:
            returnGame(member, games, records, history);
            break;
        case 3:
            displaySummary(member, games, records, history);
            break;
        case 4: {
            string targetID;
            cout << "\n--- Write a Game Review ---\n";

            // Validate Game ID is not empty
            do {
                cout << "Enter Game ID to review: ";
                getline(cin, targetID);
                if (targetID.empty()) cout << "[INVALID] ID cannot be empty.\n";
            } while (targetID.empty());

            Game* targetGame = games.findByGameID(targetID);

            if (targetGame != nullptr) {
                int rating;
                string comment;

                // --- Rating Validation ---
                while (true) {
                    cout << "Rating (1-5 stars): ";
                    if (cin >> rating && rating >= 1 && rating <= 5) {
                        cin.ignore(1000, '\n');
                        break;
                    }
                    else {
                        cout << "[INVALID] Please enter a number 1-5.\n";
                        cin.clear();
                        cin.ignore(1000, '\n');
                    }
                }

                // --- Comment Validation (Ensures it's not empty) ---
                while (true) {
                    cout << "Write your review: ";
                    getline(cin, comment);

                    // Check if they just pressed enter
                    if (comment.length() > 0 && comment[0] != ' ') {
                        break;
                    }
                    cout << "[INVALID] Review comment cannot be empty.\n";
                }

                targetGame->addReview(member.getName(), comment, rating);
                CSVHandler::saveReviews("reviews.csv", games);

                cout << "\n[SUCCESS] Your review has been submitted!\n";
            }
            else {
                cout << "[ERROR] Game ID not found.\n";
            }
            break;
        }
        case 5: { // View Reviews for a Specific Game
            string targetID;
            cout << "\n--- View Game Reviews ---\n";
            cout << "Enter Game ID: ";
            cin >> targetID;
            cin.ignore();

            Game* targetGame = games.findByGameID(targetID);
            if (targetGame != nullptr) {
                cout << "\n=========================================";
                cout << "\nReviews for: " << targetGame->getTitle();
                cout << "\n=========================================\n";

                // This calls the BST In-Order Traversal we wrote
                targetGame->displayReviews();

                cout << "=========================================\n";
            }
            else {
                cout << "[ERROR] Game ID " << targetID << " not found.\n";
            }
            break;
        }
        case 0:
            cout << "Logging out...\n";
            // Save browse history for this user
            CSVHandler::saveBrowseHistory("browse_history.csv", member.getUserID(), history);
            cout << "Returning to Main Menu.\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
            break;
        }
    }
}

