/******************************************************************************
 * Team Member: Ashton, Caden 
 * Group:7 
 * Student IDs: S10267643, S10267163
 * * Highlighted Features:
 * - Review System: Implemented using a Binary Search Tree (BST) for O(log n)
 * efficiency and sorted data display.
 * - Persistence: Custom non-STL CSV serialization/deserialization logic.
 * - Dynamic Memory Management: Custom Dynamic Array and Linked List structures.]
 *****************************************************************************/



#include "Game.h"
#include <iostream>

using namespace std;

Game::Game() {
    gameID = ""; title = "";
    minPlayers = maxPlayers = year = 0;
    totalCopies = availableCopies = 0;
    reviewRoot = nullptr;
}

Game::Game(string id, string t, int minP, int maxP, int y, int total, int avail) {
    gameID = id; title = t;
    minPlayers = minP; maxPlayers = maxP;
    year = y;
    totalCopies = total;
    availableCopies = avail;
    reviewRoot = nullptr;
}

Game::~Game() {
    clearTree(reviewRoot);
}

// --- BST LOGIC ---
/**
 * Inserts a new review into the BST recursively.
 * @param node The current root of the subtree.
 * @param name The name of the member writing the review.
 * @param comm The text content of the review.
 * @param rate The numerical rating (1-5).
 * @return The updated pointer to the subtree root.
 */
ReviewNode* Game::insertRecursive(ReviewNode* node, string name, string comm, int rate) {
    if (node == nullptr) {
        return new ReviewNode(gameID, name, comm, rate); // Creation
    }
    // Accessing node->rating is only safe AFTER the nullptr check above
    if (rate < node->rating)
        node->left = insertRecursive(node->left, name, comm, rate);
    else
        node->right = insertRecursive(node->right, name, comm, rate);
    return node;
}


/**
 * Performs an In-Order traversal to print reviews sorted by rating.
 * @param node The current node being visited.
 * @return void
 */
void Game::printInOrder(ReviewNode* node) const {
    if (node == nullptr) return;
    printInOrder(node->left);
    cout << "   [" << node->rating << "/5] " << node->memberName << ": " << node->comment << endl;
    printInOrder(node->right);
}

void Game::clearTree(ReviewNode* node) {
    if (node == nullptr) return;
    clearTree(node->left);
    clearTree(node->right);
    delete node;
}

// --- PERSISTENCE LOGIC (Non-STL) ---

void Game::saveReviewsRecursive(ofstream& file, ReviewNode* node) const {
    if (node == nullptr) return;

    // Write in format: GameID,MemberName,Rating,Comment
    // Pre-order (Root, Left, Right) is best for rebuilding the tree
    file << gameID << "," << node->memberName << ","
        << node->rating << "," << node->comment << "\n";

    saveReviewsRecursive(file, node->left);
    saveReviewsRecursive(file, node->right);
}

/**
 * Saves the BST structure to a file using Pre-Order Traversal (Root-Left-Right).
 * This ensures the tree shape is preserved when reloaded.
 * @param file The output file stream to write to.
 * @return void
 */
void Game::saveReviews(ofstream& file) const {
    if (reviewRoot != nullptr) {
        saveReviewsRecursive(file, reviewRoot);
    }
}

// --- PUBLIC INTERFACE ---

void Game::addReview(string name, string comm, int rate) {
    reviewRoot = insertRecursive(reviewRoot, name, comm, rate);
}

void Game::displayReviews() const {
    if (reviewRoot == nullptr) cout << "   (No reviews yet)\n";
    else printInOrder(reviewRoot);
}


string Game::getGameID() const { return gameID; }
string Game::getTitle() const { return title; }
int Game::getMinPlayers() const { return minPlayers; }
int Game::getMaxPlayers() const { return maxPlayers; }
int Game::getYear() const { return year; }
int Game::getTotalCopies() const { return totalCopies; }
int Game::getAvailableCopies() const { return availableCopies; }

void Game::setAvailableCopies(int avail) { availableCopies = avail; }
void Game::incrementAvailable() { if (availableCopies < totalCopies) availableCopies++; }
void Game::decrementAvailable() { if (availableCopies > 0) availableCopies--; }

void Game::print() const {
    cout << "ID: " << gameID << " | Title: " << title << endl;
    cout << "Players: " << minPlayers << "-" << maxPlayers << " | Year: " << year << endl;
    cout << "Stock: " << availableCopies << "/" << totalCopies << endl;
    cout << "Reviews:" << endl;
    displayReviews();
    cout << "-------------------------------" << endl;
}