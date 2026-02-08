/******************************************************************************
 * Team Member: Ashton, Caden
 * Group: 7
 * Student IDs: S10267643, S10267163
 * Highlighted Features:
 * - Transaction Data Encapsulation: Stores complete borrow-return lifecycle data
 * including user ID, game ID, borrow date, and return date for audit trails.
 * - Status Tracking: Distinguishes between active borrows (empty return date)
 * and completed transactions with simple isReturned() check.
 * - Flexible Display: Supports printing with optional game names for cleaner
 * human-readable output in reports.
 *****************************************************************************/

#include "BorrowRecord.h"
#include <iostream>
using namespace std;

BorrowRecord::BorrowRecord() {
    recordID = "";
    userID = "";
    gameID = "";
    borrowDate = "";
    returnDate = "";
}

BorrowRecord::BorrowRecord(string rid, string uid, string gid, string bDate, string rDate) {
    recordID = rid;
    userID = uid;
    gameID = gid;
    borrowDate = bDate;
    returnDate = rDate;
}

string BorrowRecord::getRecordID() const { return recordID; }
string BorrowRecord::getUserID() const { return userID; }
string BorrowRecord::getGameID() const { return gameID; }
string BorrowRecord::getBorrowDate() const { return borrowDate; }
string BorrowRecord::getReturnDate() const { return returnDate; }

bool BorrowRecord::isReturned() const { return returnDate != ""; }

void BorrowRecord::setReturnDate(string rDate) { returnDate = rDate; }

void BorrowRecord::print(const string& gameName) const {
    cout << "Record ID: " << recordID << "\n";
    cout << "Game: " << gameName << "\n";
    cout << "Game ID: " << gameID << "\n";
    cout << "Borrow Date: " << borrowDate << "\n";
    cout << "Return Date: " << (returnDate == "" ? "(not returned)" : returnDate) << "\n";
}
