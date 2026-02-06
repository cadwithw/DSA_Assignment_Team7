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
    int newCapacity = capacity * 2;
    Game* newData = new Game[newCapacity];

    for (int i = 0; i < count; i++) {
        newData[i] = data[i];
    }

    delete[] data;
    data = newData;
    capacity = newCapacity;
}

int GameDynamicArray::size() const {
    return count;
}

bool GameDynamicArray::isEmpty() const {
    return count == 0;
}

Game GameDynamicArray::get(int index) const {
    return data[index];
}

Game* GameDynamicArray::getPtr(int index) {
    return &data[index];
}

void GameDynamicArray::add(const Game& g) {
    if (count >= capacity) {
        resize();
    }
    data[count] = g;
    count++;
}

int GameDynamicArray::findIndexByGameID(const string& gameID) const {
    for (int i = 0; i < count; i++) {
        if (data[i].getGameID() == gameID) {
            return i;
        }
    }
    return -1;
}

Game* GameDynamicArray::findByGameID(const string& gameID) {
    int index = findIndexByGameID(gameID);
    if (index == -1) {
        return nullptr;
    }
    return &data[index];
}

bool GameDynamicArray::removeByGameID(const string& gameID) {
    int index = findIndexByGameID(gameID);
    if (index == -1) {
        return false;
    }

    for (int i = index; i < count - 1; i++) {
        data[i] = data[i + 1];
    }

    count--;
    return true;
}

void GameDynamicArray::printAll() const {
    for (int i = 0; i < count; i++) {
        data[i].print();
        cout << "----------------------\n";
    }
}
