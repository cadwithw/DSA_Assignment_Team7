#ifndef GAMEDYNAMICARRAY_H
#define GAMEDYNAMICARRAY_H

#include "Game.h"
#include <string>
using namespace std;

// Array-based implementation of the List ADT for Game objects
class GameDynamicArray {
private:
    Game* data;
    int capacity;
    int count;

    void resize();

public:
    GameDynamicArray();
    ~GameDynamicArray();

    // Basic list operations
    int size() const;
    bool isEmpty() const;

    Game get(int index) const;
    Game* getPtr(int index);

    void add(const Game& g);

    // Search operations
    int findIndexByGameID(const string& gameID) const;
    Game* findByGameID(const string& gameID);

    // Remove operation
    bool removeByGameID(const string& gameID);

    // Debug / display
    void printAll() const;
};

#endif
