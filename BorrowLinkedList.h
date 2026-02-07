#ifndef BORROWLINKEDLIST_H
#define BORROWLINKEDLIST_H

#include "BorrowRecord.h"

class BorrowLinkedList {
private:
    struct Node {
        BorrowRecord data;
        Node* next;
    };

    Node* head;

public:
    BorrowLinkedList();
    ~BorrowLinkedList();

    void insertFront(const BorrowRecord& br);
    BorrowRecord* findActiveBorrow(const string& userID, const string& gameID);

    void printAll() const;
    void printByUser(const string& userID) const;
    void writeToCSV(ofstream& file) const;
    bool isEmpty() const { return head == nullptr; }
    
    // New helper methods for summary statistics
    int countBorrowsByUser(const string& userID) const;
    int countReturnsByUser(const string& userID) const;
    int countActiveBorrowsByUser(const string& userID) const;
};

#endif
