#ifndef GAME_H
#define GAME_H

#include <string>
#include <fstream>

using namespace std;

struct ReviewNode {
    string gameID;      // Add this field
    string memberName;
    string comment;
    int rating;
    ReviewNode* left, * right;

    // Constructor MUST take 4 arguments and initialize pointers to nullptr
    ReviewNode(string gID, string n, string c, int r) :
        gameID(gID), memberName(n), comment(c), rating(r),
        left(nullptr), right(nullptr) {
    }
};

class Game {
private:
    string gameID;
    string title;
    int minPlayers;
    int maxPlayers;
    int year;
    int totalCopies;
    int availableCopies;

    ReviewNode* reviewRoot;

    // Private Recursive Helpers for BST
    ReviewNode* insertRecursive(ReviewNode* node, string name, string comm, int rate);
    void printInOrder(ReviewNode* node) const;
    void clearTree(ReviewNode* node);
    
    // Helper to deep copy BST
    ReviewNode* copyTree(ReviewNode* node) const;

    // Recursive Helper for saving to CSV
    void saveReviewsRecursive(ofstream& file, ReviewNode* node) const;

public:
    Game();
    Game(string id, string t, int minP, int maxP, int y, int total, int avail);
    Game(const Game& other);
    Game& operator=(const Game& other);
    ~Game();

    // Getters
    string getGameID() const;
    string getTitle() const;
    int getMinPlayers() const;
    int getMaxPlayers() const;
    int getYear() const;
    int getTotalCopies() const;
    int getAvailableCopies() const;

    // Setters
    void setAvailableCopies(int avail);
    void incrementAvailable();
    void decrementAvailable();

    void print() const;
    void printWithoutReviews() const;  // New method for search results

    // Review Interface
    void addReview(string name, string comm, int rate);
    void displayReviews() const;

    //File Persistence Interface
    void saveReviews(ofstream& file) const;
};

#endif