#pragma once
#include "Game.h"
#include <string>
#include <cassert>

class GameDynamicArray {
private:
    Game** data;
    int capacity;
    int count;

    void resize();

public:
    GameDynamicArray();
    ~GameDynamicArray();

    // Prevent shallow copies that would lead to double-free/dangling pointers
    GameDynamicArray(const GameDynamicArray&) = delete;
    GameDynamicArray& operator=(const GameDynamicArray&) = delete;

    int size() const;
    bool isEmpty() const;

    // Return by reference to avoid creating temporaries (which previously caused destructor runs)
    Game& get(int index);
    const Game& get(int index) const;

    // Pointer access
    Game* getPtr(int index);

    // Ownership: add allocates a new Game on the heap
    void add(const Game& g);

    int findIndexByGameID(const std::string& gameID) const;
    Game* findByGameID(const std::string& gameID);

    bool removeByGameID(const std::string& gameID);

    void printAll() const;
};
