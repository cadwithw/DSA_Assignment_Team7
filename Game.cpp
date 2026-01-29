#include "Game.h"
#include <iostream>
using namespace std;

Game::Game() {
    gameID = "";
    title = "";
    minPlayers = maxPlayers = year = 0;
    totalCopies = availableCopies = 0;
}

Game::Game(string id, string t, int minP, int maxP, int y, int total, int avail) {
    gameID = id;
    title = t;
    minPlayers = minP;
    maxPlayers = maxP;
    year = y;
    totalCopies = total;
    availableCopies = avail;
}

string Game::getGameID() const { return gameID; }
string Game::getTitle() const { return title; }
int Game::getMinPlayers() const { return minPlayers; }
int Game::getMaxPlayers() const { return maxPlayers; }
int Game::getYear() const { return year; }
int Game::getTotalCopies() const { return totalCopies; }
int Game::getAvailableCopies() const { return availableCopies; }

void Game::setAvailableCopies(int avail) { availableCopies = avail; }

void Game::incrementAvailable() {
    if (availableCopies < totalCopies) availableCopies++;
}

void Game::decrementAvailable() {
    if (availableCopies > 0) availableCopies--;
}

void Game::print() const {
    cout << "Game ID: " << gameID << "\n";
    cout << "Title: " << title << "\n";
    cout << "Players: " << minPlayers << "-" << maxPlayers << "\n";
    cout << "Year: " << year << "\n";
    cout << "Copies: " << availableCopies << "/" << totalCopies << "\n";
}
