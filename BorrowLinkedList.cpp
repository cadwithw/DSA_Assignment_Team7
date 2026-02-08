/******************************************************************************
 * Team Member: Ashton, Caden
 * Group: 7
 * Student IDs: S10267643, S10267163
 * Highlighted Features:
 * - Linked List for Transactions: Implements a singly-linked list for efficient
 * O(1) front insertion of new borrow records without needing to reallocate.
 * - Linear Search: O(n) traversal to find active borrows and user-specific records.
 * - Transaction Persistence: Serializes all borrow/return records to CSV for
 * data persistence and audit trail generation.
 *****************************************************************************/

/// Implementation of BorrowLinkedList - A linked list-based structure for managing borrow records
/// This linked list stores all borrow and return transactions for the board game management system.
/// Each node contains a BorrowRecord with transaction details (user ID, game ID, borrow date, return date).

#include "BorrowLinkedList.h"
#include "GameDynamicArray.h"
#include <iostream>
#include <fstream>
using namespace std;

/// Constructor - Initializes an empty linked list
/// Time Complexity: O(1)
BorrowLinkedList::BorrowLinkedList() {
    head = nullptr;  // Empty list starts with nullptr head
}

/// Destructor - Deallocates all nodes in the linked list
/// Traverses the entire list and deletes each node to prevent memory leaks
/// Time Complexity: O(n) where n is the number of borrow records
BorrowLinkedList::~BorrowLinkedList() {
    Node* cur = head;
    while (cur != nullptr) {
        Node* temp = cur;
        cur = cur->next;
        delete temp;  // Free memory for current node
    }
}

/// Inserts a new borrow record at the front of the linked list
/// This is the primary insertion method. New records are always added at the front
/// for O(1) insertion time. The list is not ordered by date - newest records are at front.
/// Time Complexity: O(1)
void BorrowLinkedList::insertFront(const BorrowRecord& br) {
    Node* newNode = new Node;
    newNode->data = br;
    newNode->next = head;  // Link new node to current head
    head = newNode;        // New node becomes the new head
}

/// Searches for an active (not yet returned) borrow record for a specific user and game
/// An "active" borrow is one where the return date is empty (game not yet returned).
/// This method is used when a user wants to return a game or check current borrows.
/// Time Complexity: O(n) where n is the number of borrow records
BorrowRecord* BorrowLinkedList::findActiveBorrow(const string& userID, const string& gameID) {
    Node* cur = head;
    while (cur != nullptr) {
        // Check if this record matches the user and game, and hasn't been returned yet
        if (cur->data.getUserID() == userID &&
            cur->data.getGameID() == gameID &&
            !cur->data.isReturned()) {
            return &(cur->data);
        }
        cur = cur->next;
    }
    return nullptr;  // No active borrow found
}

/// Displays all borrow records in the linked list
/// Traverses the entire linked list and prints each record. If a games array is provided,
/// it looks up and displays the game title; otherwise shows "Unknown" for the game name.
/// Used by admin menu to view all borrowing transactions.
/// Time Complexity: O(n*m) where n is number of records and m is number of games (for lookup)
void BorrowLinkedList::printAll(GameDynamicArray* games) const {
    Node* cur = head;
    while (cur != nullptr) {
        cout << "---------------------\n";
        if (games != nullptr) {
            // Look up the game name by game ID for better readability
            string gameName = "Unknown";
            for (int i = 0; i < games->size(); i++) {
                if (games->get(i).getGameID() == cur->data.getGameID()) {
                    gameName = games->get(i).getTitle();
                    break;
                }
            }
            cur->data.print(gameName);
        } else {
            cur->data.print();
        }
        cur = cur->next;
    }
}

/// Displays all borrow records for a specific user
/// Traverses the linked list and displays only records belonging to the specified user.
/// If a games array is provided, it looks up and displays game titles.
/// Used in the member menu to show individual user's borrow history.
/// Time Complexity: O(n*m) where n is number of records and m is number of games (for lookup)
void BorrowLinkedList::printByUser(const string& userID, GameDynamicArray* games) const {
    Node* cur = head;
    while (cur != nullptr) {
        // Only print records belonging to this user
        if (cur->data.getUserID() == userID) {
            cout << "---------------------\n";
            if (games != nullptr) {
                // Look up the game name by game ID for better readability
                string gameName = "Unknown";
                for (int i = 0; i < games->size(); i++) {
                    if (games->get(i).getGameID() == cur->data.getGameID()) {
                        gameName = games->get(i).getTitle();
                        break;
                    }
                }
                cur->data.print(gameName);
            } else {
                cur->data.print();
            }
        }
        cur = cur->next;
    }
}
/// Writes all borrow records to a CSV file
/// Serializes the entire linked list to CSV format with the following columns:
/// recordID, userID, gameID, borrowDate, returnDate
/// This method is used during program shutdown to persist all borrow records.
/// Time Complexity: O(n) where n is the number of borrow records
void BorrowLinkedList::writeToCSV(ofstream& file) const {
    Node* cur = head;
    while (cur != nullptr) {
        BorrowRecord br = cur->data;

        // Write record in CSV format: recordID,userID,gameID,borrowDate,returnDate
        file << br.getRecordID() << ","
            << br.getUserID() << ","
            << br.getGameID() << ","
            << br.getBorrowDate() << ","
            << br.getReturnDate() << "\n";

        cur = cur->next;
    }
}

/// Counts total number of borrow transactions for a specific user
/// Includes both active borrows and completed borrows (with return date).
/// Used in member summary to display total borrow transactions.
/// Time Complexity: O(n) where n is the number of borrow records
int BorrowLinkedList::countBorrowsByUser(const string& userID) const {
    Node* cur = head;
    int count = 0;
    while (cur != nullptr) {
        if (cur->data.getUserID() == userID) {
            count++;
        }
        cur = cur->next;
    }
    return count;
}

/// Counts number of completed (returned) borrow transactions for a specific user
/// Only counts records where returnDate is not empty (games that were returned).
/// Used in member summary to calculate return rate percentage.
/// Time Complexity: O(n) where n is the number of borrow records
int BorrowLinkedList::countReturnsByUser(const string& userID) const {
    Node* cur = head;
    int count = 0;
    while (cur != nullptr) {
        if (cur->data.getUserID() == userID && cur->data.isReturned()) {
            count++;
        }
        cur = cur->next;
    }
    return count;
}

/// Counts number of active (not yet returned) borrow transactions for a specific user
/// Only counts records where the game has not been returned (returnDate is empty).
/// Used in member summary to display currently borrowed games count.
/// Time Complexity: O(n) where n is the number of borrow records
int BorrowLinkedList::countActiveBorrowsByUser(const string& userID) const {
    Node* cur = head;
    int count = 0;
    while (cur != nullptr) {
        if (cur->data.getUserID() == userID && !cur->data.isReturned()) {
            count++;
        }
        cur = cur->next;
    }
    return count;
}