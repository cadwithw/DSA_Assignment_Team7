#ifndef GAME_H
#define GAME_H

#include <string>
using namespace std;

class Game {
private:
    string gameID;
    string title;
    int minPlayers;
    int maxPlayers;
    int year;
    int totalCopies;
    int availableCopies;

public:
    Game();
    Game(string id, string t, int minP, int maxP, int y, int total, int avail);

    // Getters
    string getGameID() const;
    string getTitle() const;
    int getMinPlayers() const;
    int getMaxPlayers() const;
    int getYear() const;
    int getTotalCopies() const;
    int getAvailableCopies() const;

    // Setters / updates
    void setAvailableCopies(int avail);
    void incrementAvailable();
    void decrementAvailable();

    void print() const;
};

#endif
