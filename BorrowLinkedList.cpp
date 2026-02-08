#include "BorrowLinkedList.h"
#include "GameDynamicArray.h"
#include <iostream>
#include <fstream>
using namespace std;

BorrowLinkedList::BorrowLinkedList() {
    head = nullptr;
}

BorrowLinkedList::~BorrowLinkedList() {
    Node* cur = head;
    while (cur != nullptr) {
        Node* temp = cur;
        cur = cur->next;
        delete temp;
    }
}

void BorrowLinkedList::insertFront(const BorrowRecord& br) {
    Node* newNode = new Node;
    newNode->data = br;
    newNode->next = head;
    head = newNode;
}

BorrowRecord* BorrowLinkedList::findActiveBorrow(const string& userID, const string& gameID) {
    Node* cur = head;
    while (cur != nullptr) {
        if (cur->data.getUserID() == userID &&
            cur->data.getGameID() == gameID &&
            !cur->data.isReturned()) {
            return &(cur->data);
        }
        cur = cur->next;
    }
    return nullptr;
}

void BorrowLinkedList::printAll(GameDynamicArray* games) const {
    Node* cur = head;
    while (cur != nullptr) {
        cout << "---------------------\n";
        if (games != nullptr) {
            // Look up the game name by game ID
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

void BorrowLinkedList::printByUser(const string& userID, GameDynamicArray* games) const {
    Node* cur = head;
    while (cur != nullptr) {
        if (cur->data.getUserID() == userID) {
            cout << "---------------------\n";
            if (games != nullptr) {
                // Look up the game name by game ID
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
void BorrowLinkedList::writeToCSV(ofstream& file) const {
    Node* cur = head;
    while (cur != nullptr) {
        BorrowRecord br = cur->data;

        file << br.getRecordID() << ","
            << br.getUserID() << ","
            << br.getGameID() << ","
            << br.getBorrowDate() << ","
            << br.getReturnDate() << "\n";

        cur = cur->next;
    }
}

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