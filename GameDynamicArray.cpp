#include "GameDynamicArray.h"
#include <iostream>
#include <cassert>

using namespace std;

GameDynamicArray::GameDynamicArray() {
    capacity = 10;
    count = 0;
    data = new Game*[capacity];
    for (int i = 0; i < capacity; ++i) data[i] = nullptr;
}

GameDynamicArray::~GameDynamicArray() {
    for (int i = 0; i < count; i++) {
        delete data[i]; // free each allocated Game object
        data[i] = nullptr;
    }
    delete[] data;
    data = nullptr;
    capacity = 0;
    count = 0;
}

void GameDynamicArray::resize() {
    int newCapacity = (capacity > 0) ? capacity * 2 : 10;
    Game** newData = new Game*[newCapacity];
    for (int i = 0; i < newCapacity; ++i) newData[i] = nullptr;

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

Game& GameDynamicArray::get(int index) {
    assert(index >= 0 && index < count);
    return *data[index];
}

const Game& GameDynamicArray::get(int index) const {
    assert(index >= 0 && index < count);
    return *data[index];
}

Game* GameDynamicArray::getPtr(int index) {
    if (index < 0 || index >= count) return nullptr;
    return data[index];
}

void GameDynamicArray::add(const Game& g) {
    if (count >= capacity) {
        resize();
    }
    data[count] = new Game(g); // allocate a new Game on the heap
    ++count;
}

int GameDynamicArray::findIndexByGameID(const string& gameID) const {
    for (int i = 0; i < count; i++) {
        if (data[i] != nullptr && data[i]->getGameID() == gameID) {
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
    return data[index];
}

bool GameDynamicArray::removeByGameID(const string& gameID) {
    int index = findIndexByGameID(gameID);
    if (index == -1) {
        return false;
    }

    delete data[index];
    data[index] = nullptr;

    for (int i = index; i < count - 1; i++) {
        data[i] = data[i + 1];
    }
    data[count - 1] = nullptr;
    --count;
    return true;
}

void GameDynamicArray::printAll() const {
    for (int i = 0; i < count; i++) {
        if (data[i] != nullptr) {
            data[i]->print();
            cout << "----------------------\n";
        }
    }
}
