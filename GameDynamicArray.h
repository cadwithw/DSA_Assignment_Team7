#ifndef GAMEDYNAMICARRAY_H
#define GAMEDYNAMICARRAY_H

#include "Game.h"

class GameDynamicArray {
private:
    Game* data;
    int capacity;
    int count;

    void resize();

public:
    GameDynamicArray();
    ~GameDynamicArray();

    int size() const;
    Game get(int index) const;
    Game* getPtr(int index);

    void add(const Game& g);
    bool removeByGameID(const string& gameID);

    int findIndexByGameID(const string& gameID) const;

    void printAll() const;
};

#endif
