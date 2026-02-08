#ifndef BROWSEHISTORY_H
#define BROWSEHISTORY_H

#include "Game.h"
#include <string>
using namespace std;

class GameDynamicArray;  // Forward declaration

class BrowseHistory {
private:
    struct Node {
        Game game;
        string viewedDate;
        Node* next;
    };
    
    Node* front;  // Front of queue (oldest item)
    Node* rear;   // Rear of queue (newest item)
    int count;    // Current number of items in queue
    static const int MAX_HISTORY_SIZE = 5;  // Maximum items to keep
    
    void dequeue();  // Remove oldest item when limit exceeded
    
public:
    BrowseHistory();
    ~BrowseHistory();
    
    void enqueue(const Game& game, const string& viewedDate);  // Add new game to queue
    bool isEmpty() const;
    int getSize() const;
    void displayHistory(GameDynamicArray* games = nullptr) const;
    Game* getGameAt(int index) const;
    string getGameIDAt(int index) const;
    string getViewedDateAt(int index) const;
    
    // For CSV persistence
    void clearHistory();
};

#endif
