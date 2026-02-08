#ifndef BORROWRECORD_H
#define BORROWRECORD_H

#include <string>
using namespace std;

class BorrowRecord {
private:
    string recordID;
    string userID;
    string gameID;
    string borrowDate;
    string returnDate; // empty = not returned

public:
    BorrowRecord();
    BorrowRecord(string rid, string uid, string gid, string bDate, string rDate);

    string getRecordID() const;
    string getUserID() const;
    string getGameID() const;
    string getBorrowDate() const;
    string getReturnDate() const;

    bool isReturned() const;
    void setReturnDate(string rDate);

    void print(const string& gameName = "Unknown") const;
};

#endif
