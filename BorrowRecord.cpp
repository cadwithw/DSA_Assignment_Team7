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
