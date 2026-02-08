/******************************************************************************
 * Team Member: Ashton, Caden
 * Group: 7
 * Student IDs: S10267643, S10267163
 * Highlighted Features:
 * - Review System: Implemented using a Binary Search Tree (BST) for O(log n)
 * efficiency and sorted data display.
 * - Persistence: Custom non-STL CSV serialization/deserialization logic.
 * - Dynamic Memory Management: Deep copy implementation for BST nodes during
 * object duplication and assignment.
 *****************************************************************************/

#include "Game.h"
#include <iostream>

using namespace std;

/**
 * Default Constructor.
 * Initializes an empty Game object with null/zero values.
 */
Game::Game() {
    gameID = ""; title = "";
    minPlayers = maxPlayers = year = 0;
    totalCopies = availableCopies = 0;
    reviewRoot = nullptr;
}

/**
 * Parameterized Constructor.
 * @param id Unique Game ID.
 * @param t Title of the board game.
 * @param minP Minimum players.
 * @param maxP Maximum players.
 * @param y Release year.
 * @param total Total inventory stock.
 * @param avail Currently available copies.
 */
Game::Game(string id, string t, int minP, int maxP, int y, int total, int avail) {
    gameID = id; title = t;
    minPlayers = minP; maxPlayers = maxP;
    year = y;
    totalCopies = total;
    availableCopies = avail;
    reviewRoot = nullptr;
}

/**
 * Copy Constructor (Deep Copy).
 * Ensures that when a Game object is copied, a new BST of reviews is
 * created rather than sharing a pointer to the original tree.
 */
Game::Game(const Game& other) {
    gameID = other.gameID;
    title = other.title;
    minPlayers = other.minPlayers;
    maxPlayers = other.maxPlayers;
    year = other.year;
    totalCopies = other.totalCopies;
    availableCopies = other.availableCopies;
    reviewRoot = copyTree(other.reviewRoot);
}

/**
 * Overloaded Assignment Operator (Deep Copy).
 * Cleans up existing memory before copying data from another Game object.
 */
Game& Game::operator=(const Game& other) {
    if (this == &other) return *this;

    clearTree(reviewRoot);

    gameID = other.gameID;
    title = other.title;
    minPlayers = other.minPlayers;
    maxPlayers = other.maxPlayers;
    year = other.year;
    totalCopies = other.totalCopies;
    availableCopies = other.availableCopies;
    reviewRoot = copyTree(other.reviewRoot);

    return *this;
}

/**
 * Destructor.
 * Triggers a recursive cleanup of the review BST to prevent memory leaks.
 */
Game::~Game() {
    clearTree(reviewRoot);
}

// --- BST LOGIC ---

/**
 * Recursively clones a BST.
 * @param node The root of the tree to copy.
 * @return A pointer to the new root of the cloned tree.
 */
ReviewNode* Game::copyTree(ReviewNode* node) const {
    if (node == nullptr) return nullptr;

    ReviewNode* newNode = new ReviewNode(node->gameID, node->memberName, node->comment, node->rating);
    newNode->left = copyTree(node->left);
    newNode->right = copyTree(node->right);

    return newNode;
}

/**
 * Inserts a new review into the BST based on numerical rating.
 * @param node Current node in traversal.
 * @param name Author of the review.
 * @param comm Review text.
 * @param rate Rating score.
 * @return Updated pointer to the node.
 */
ReviewNode* Game::insertRecursive(ReviewNode* node, string name, string comm, int rate) {
    if (node == nullptr) {
        return new ReviewNode(gameID, name, comm, rate);
    }
    if (rate < node->rating)
        node->left = insertRecursive(node->left, name, comm, rate);
    else
        node->right = insertRecursive(node->right, name, comm, rate);
    return node;
}

/**
 * Performs an In-Order traversal to print reviews sorted by rating (Ascending).
 * @param node The current node being visited.
 */
void Game::printInOrder(ReviewNode* node) const {
    if (node == nullptr) return;
    printInOrder(node->left);
    cout << "    [" << node->rating << "/5] " << node->memberName << ": " << node->comment << endl;
    printInOrder(node->right);
}

/**
 * Post-Order traversal to delete every node in the BST.
 * @param node The current node to delete.
 */
void Game::clearTree(ReviewNode* node) {
    if (node == nullptr) return;
    clearTree(node->left);
    clearTree(node->right);
    delete node;
}

// --- PERSISTENCE LOGIC ---

/**
 * Pre-Order traversal for saving reviews.
 * Pre-order is used so that when the file is read, the tree is rebuilt
 * with the same root and structure as before.
 */
void Game::saveReviewsRecursive(ofstream& file, ReviewNode* node) const {
    if (node == nullptr) return;

    file << gameID << "," << node->memberName << ","
        << node->rating << ",\"" << node->comment << "\"\n";

    saveReviewsRecursive(file, node->left);
    saveReviewsRecursive(file, node->right);
}

/**
 * Interface function to initiate saving the review tree to a file stream.
 */
void Game::saveReviews(ofstream& file) const {
    if (reviewRoot != nullptr) {
        saveReviewsRecursive(file, reviewRoot);
    }
}

// --- PUBLIC INTERFACE ---

/** Adds a new review to the game's internal BST. */
void Game::addReview(string name, string comm, int rate) {
    reviewRoot = insertRecursive(reviewRoot, name, comm, rate);
}

/** Displays all reviews for this game, sorted by rating score. */
void Game::displayReviews() const {
    if (reviewRoot == nullptr) cout << "   (No reviews yet)\n";
    else printInOrder(reviewRoot);
}

// --- GETTERS ---
string Game::getGameID() const { return gameID; }
string Game::getTitle() const { return title; }
int Game::getMinPlayers() const { return minPlayers; }
int Game::getMaxPlayers() const { return maxPlayers; }
int Game::getYear() const { return year; }
int Game::getTotalCopies() const { return totalCopies; }
int Game::getAvailableCopies() const { return availableCopies; }

// --- SETTERS / UPDATERS ---
void Game::setAvailableCopies(int avail) { availableCopies = avail; }
void Game::incrementAvailable() { if (availableCopies < totalCopies) availableCopies++; }
void Game::decrementAvailable() { if (availableCopies > 0) availableCopies--; }

/** Prints full game details, including inventory status and all reviews. */
void Game::print() const {
    cout << "ID: " << gameID << " | Title: " << title << endl;
    cout << "Players: " << minPlayers << "-" << maxPlayers << " | Year: " << year << endl;
    cout << "Stock: " << availableCopies << "/" << totalCopies << endl;
    cout << "Reviews:" << endl;
    displayReviews();
    cout << "-------------------------------" << endl;
}

/** Prints game details without reviews (for search results display). */
void Game::printWithoutReviews() const {
    cout << "ID: " << gameID << " | Title: " << title << endl;
    cout << "Players: " << minPlayers << "-" << maxPlayers << " | Year: " << year << endl;
    cout << "Stock: " << availableCopies << "/" << totalCopies << endl;
}