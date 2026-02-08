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

ReviewNode* Game::insertRecursive(ReviewNode* node, string name, string comm, int rate) {
    if (node == nullptr) return new ReviewNode(name, comm, rate);

    // Sort by rating: lower left, higher/equal right
    if (rate < node->rating)
        node->left = insertRecursive(node->left, name, comm, rate);
    else
        node->right = insertRecursive(node->right, name, comm, rate);

    return node;
}

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

// --- EXISTING GETTERS/SETTERS ---

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