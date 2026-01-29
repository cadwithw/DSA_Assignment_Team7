#include "GameDynamicArray.h"
#include <iostream>
using namespace std;

GameDynamicArray::GameDynamicArray() {
    capacity = 10;
    count = 0;
    data = new Game[capacity];
}

GameDynamicArray::~GameDynamicArray() {
    delete[] data;
}

void GameDynamicArray::resize() {
    int newCap = capacity * 2;
    Game* newData = new Game[newCap];

    for (int i = 0; i < count; i++) {
        newData[i] = data[i];
    }

    delete[] data;
    data = newData;
    capacity = newCap;
}

int GameDynamicArray::size() const {
    return count;
}

Game GameDynamicArray::get(int index) const {
    return data[index];
}

Game* GameDynamicArray::getPtr(int index) {
    return &data[index];
}

void GameDynamicArray::add(const Game& g) {
    if (count >= capacity) resize();
    data[count] = g;
    count++;
}

int GameDynamicArray::findIndexByGameID(const string& gameID) const {
    for (int i = 0; i < count; i++) {
        if (data[i].getGameID() == gameID) return i;
    }
    return -1;
}

bool GameDynamicArray::removeByGameID(const string& gameID) {
    int idx = findIndexByGameID(gameID);
    if (idx == -1) return false;

    for (int i = idx; i < count - 1; i++) {
        data[i] = data[i + 1];
    }
    count--;
    return true;
}

void GameDynamicArray::printAll() const {
    for (int i = 0; i < count; i++) {
        cout << "---------------------\n";
        data[i].print();
    }
}
