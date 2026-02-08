/******************************************************************************
 * Team Member: Ashton, Caden
 * Group: 7
 * Student IDs: S10267643, S10267163
 * Highlighted Features:
 * - Dynamic Memory Management: Manages a heap-allocated array of pointers to
 * Game objects, allowing for polymorphic behavior if extended.
 * - Deep Copy Addition: Uses the Game copy constructor when adding elements
 * to ensure the BST and other internal structures are properly duplicated.
 * - Optimized Removal: Implements element shifting to maintain a contiguous
 * array, ensuring search operations remain consistent.
 *****************************************************************************/

#include "GameDynamicArray.h"
#include <iostream>
#include <cassert>

using namespace std;

/**
 * Default constructor.
 * Initializes the array with a capacity of 10 and nullifies all pointers.
 */
GameDynamicArray::GameDynamicArray() {
    capacity = 10;
    count = 0;
    data = new Game * [capacity];
    for (int i = 0; i < capacity; ++i) data[i] = nullptr;
}

/**
 * Destructor.
 * Ensures every individual Game object is deleted before freeing the pointer array.
 */
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

/**
 * Expands the array capacity when full (usually doubling it).
 * Reallocates memory and migrates pointers to the new block.
 */
void GameDynamicArray::resize() {
    int newCapacity = (capacity > 0) ? capacity * 2 : 10;
    Game** newData = new Game * [newCapacity];
    for (int i = 0; i < newCapacity; ++i) newData[i] = nullptr;

    for (int i = 0; i < count; i++) {
        newData[i] = data[i];
    }

    delete[] data;
    data = newData;
    capacity = newCapacity;
}

/** @return The current number of elements in the array. */
int GameDynamicArray::size() const {
    return count;
}

/** @return True if the array contains no elements. */
bool GameDynamicArray::isEmpty() const {
    return count == 0;
}

/**
 * Accesses a Game reference at the specified index.
 * Uses assertions for safety in debug mode.
 * @param index The position to access.
 * @return Reference to the Game object.
 */
Game& GameDynamicArray::get(int index) {
    assert(index >= 0 && index < count);
    return *data[index];
}

/**
 * Constant version of get() for read-only access.
 * @param index The position to access.
 * @return Constant reference to the Game object.
 */
const Game& GameDynamicArray::get(int index) const {
    assert(index >= 0 && index < count);
    return *data[index];
}

/**
 * Returns the raw pointer to the Game object at an index.
 * @param index The position to access.
 * @return Game pointer or nullptr if index is out of bounds.
 */
Game* GameDynamicArray::getPtr(int index) {
    if (index < 0 || index >= count) return nullptr;
    return data[index];
}

/**
 * Adds a copy of a Game object to the array.
 * Triggers resize() if necessary and allocates the Game on the heap.
 * @param g The Game object to duplicate and add.
 */
void GameDynamicArray::add(const Game& g) {
    if (count >= capacity) {
        resize();
    }
    data[count] = new Game(g); // allocate a new Game on the heap
    ++count;
}

/**
 * Linear search for a game's index by its unique ID.
 * @param gameID The string ID to search for.
 * @return The index if found, -1 otherwise.
 */
int GameDynamicArray::findIndexByGameID(const string& gameID) const {
    for (int i = 0; i < count; i++) {
        if (data[i] != nullptr && data[i]->getGameID() == gameID) {
            return i;
        }
    }
    return -1;
}

/**
 * Finds a game by ID and returns its pointer.
 * @param gameID The string ID to search for.
 * @return Pointer to the Game or nullptr if not found.
 */
Game* GameDynamicArray::findByGameID(const string& gameID) {
    int index = findIndexByGameID(gameID);
    if (index == -1) {
        return nullptr;
    }
    return data[index];
}

/**
 * Removes a game from the array and shifts remaining elements to fill the gap.
 * Frees the memory of the removed Game object.
 * @param gameID The string ID of the game to delete.
 * @return True if successful, false if the ID was not found.
 */
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

/**
 * Iterates through the array and calls the print() method for each valid Game.
 */
void GameDynamicArray::printAll() const {
    for (int i = 0; i < count; i++) {
        if (data[i] != nullptr) {
            data[i]->print();
            cout << "----------------------\n";
        }
    }
}