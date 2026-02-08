/******************************************************************************
 * Team Member: Ashton, Caden
 * Group: 7
 * Student IDs: S10267643, S10267163
 * Highlighted Features:
 * - Queue ADT with Auto-Limiting: Implements a linked list-based queue that
 * maintains only the 5 most recent game views with O(1) enqueue/dequeue.
 * - Real-Time Stock Lookup: Fetches current game stock from main games array
 * to display live inventory instead of stale cached data.
 * - CSV Persistence: Browse history persists per-user across sessions, allowing
 * users to see their viewing history on re-login without data loss.
 *****************************************************************************/

#include "BrowseHistory.h"
#include "GameDynamicArray.h"
#include <iostream>
using namespace std;

BrowseHistory::BrowseHistory() {
    front = nullptr;
    rear = nullptr;
    count = 0;
}

BrowseHistory::~BrowseHistory() {
    clearHistory();
}

void BrowseHistory::dequeue() {
    if (front == nullptr) return;
    
    Node* temp = front;
    front = front->next;
    delete temp;
    count--;
    
    // If queue becomes empty, reset rear
    if (front == nullptr) {
        rear = nullptr;
    }
}

void BrowseHistory::enqueue(const Game& game, const string& viewedDate) {
    Node* newNode = new Node;
    newNode->game = game;
    newNode->viewedDate = viewedDate;
    newNode->next = nullptr;
    
    // If queue is empty, new node is both front and rear
    if (front == nullptr) {
        front = newNode;
        rear = newNode;
    } else {
        // Add to rear
        rear->next = newNode;
        rear = newNode;
    }
    count++;
    
    // If size exceeds MAX_HISTORY_SIZE, remove oldest item from front
    if (count > MAX_HISTORY_SIZE) {
        dequeue();
    }
}

bool BrowseHistory::isEmpty() const {
    return front == nullptr;
}

int BrowseHistory::getSize() const {
    return count;
}

void BrowseHistory::displayHistory(GameDynamicArray* games) const {
    if (isEmpty()) {
        cout << "You haven't viewed any games yet.\n";
        return;
    }
    
    // Collect all nodes
    Node* nodes[5];
    int idx = 0;
    Node* temp = front;
    while (temp != nullptr && idx < 5) {
        nodes[idx] = temp;
        temp = temp->next;
        idx++;
    }
    
    cout << "\n--- Your Browse History (Most Recent First) ---\n";
    
    // Display in reverse order (most recent first)
    for (int i = idx - 1; i >= 0; i--) {
        string gameID = nodes[i]->game.getGameID();
        int availableCopies = nodes[i]->game.getAvailableCopies();
        int totalCopies = nodes[i]->game.getTotalCopies();
        
        // If games array is provided, fetch current stock info
        if (games != nullptr) {
            Game* currentGame = nullptr;
            for (int j = 0; j < games->size(); j++) {
                if (games->get(j).getGameID() == gameID) {
                    currentGame = &(games->getPtr(j)[0]);
                    availableCopies = currentGame->getAvailableCopies();
                    totalCopies = currentGame->getTotalCopies();
                    break;
                }
            }
        }
        
        cout << "[" << (idx - i) << "] " << nodes[i]->game.getTitle() << "\n";
        cout << "    Game ID: " << gameID << "\n";
        cout << "    Viewed: " << nodes[i]->viewedDate << "\n";
        cout << "    Players: " << nodes[i]->game.getMinPlayers() << "-" 
             << nodes[i]->game.getMaxPlayers() << "\n";
        cout << "    Stock: " << availableCopies << "/" << totalCopies << "\n";
        cout << "-----------------------\n";
    }
}

Game* BrowseHistory::getGameAt(int index) const {
    if (index < 0 || index >= count) {
        return nullptr;
    }
    
    Node* current = front;
    int currentIndex = 0;
    
    while (current != nullptr) {
        if (currentIndex == index) {
            return &(current->game);
        }
        current = current->next;
        currentIndex++;
    }
    return nullptr;
}

string BrowseHistory::getViewedDateAt(int index) const {
    if (index < 0 || index >= count) {
        return "";
    }
    
    Node* current = front;
    int currentIndex = 0;
    
    while (current != nullptr) {
        if (currentIndex == index) {
            return current->viewedDate;
        }
        current = current->next;
        currentIndex++;
    }
    return "";
}

string BrowseHistory::getGameIDAt(int index) const {
    if (index < 0 || index >= count) {
        return "";
    }
    
    Node* current = front;
    int currentIndex = 0;
    
    while (current != nullptr) {
        if (currentIndex == index) {
            return current->game.getGameID();
        }
        current = current->next;
        currentIndex++;
    }
    return "";
}

void BrowseHistory::clearHistory() {
    Node* current = front;
    while (current != nullptr) {
        Node* temp = current;
        current = current->next;
        delete temp;
    }
    front = nullptr;
    rear = nullptr;
    count = 0;
}
