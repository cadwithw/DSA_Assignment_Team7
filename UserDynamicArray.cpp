/******************************************************************************
 * Team Member: Ashton, Caden
 * Group: 7
 * Student IDs: S10267643, S10267163
 * Highlighted Features:
 * - Dynamic Resizing: Implements a growth factor of 2x to ensure efficient
 * amortized O(1) addition of user accounts.
 * - Linear Search: Optimized UserID lookup used for authentication and
 * account management.
 * - Contiguous Storage: Uses direct object storage for better cache locality
 * when iterating through the user directory.
 *****************************************************************************/

#include "UserDynamicArray.h"
#include <iostream>
using namespace std;

/**
 * Default constructor for UserDynamicArray.
 * Allocates an initial capacity of 10 User objects on the heap.
 */
UserDynamicArray::UserDynamicArray() {
    capacity = 10;
    count = 0;
    data = new User[capacity];
}

/**
 * Destructor for UserDynamicArray.
 * Frees the dynamically allocated array of User objects.
 */
UserDynamicArray::~UserDynamicArray() {
    delete[] data;
}

/**
 * Doubles the current capacity of the array when full.
 * Transfers existing User objects to the new memory block.
 */
void UserDynamicArray::resize() {
    int newCap = capacity * 2;
    User* newData = new User[newCap];

    for (int i = 0; i < count; i++) {
        newData[i] = data[i];
    }

    delete[] data;
    data = newData;
    capacity = newCap;
}

/**
 * @return The number of users currently stored in the array.
 */
int UserDynamicArray::size() const {
    return count;
}

/**
 * Retrieves a copy of the User object at the specified index.
 * @param index The position in the array.
 * @return A copy of the User object.
 */
User UserDynamicArray::get(int index) const {
    return data[index];
}

/**
 * Retrieves a pointer to a User object at a specific index.
 * Allows for direct modification of the user data within the array.
 * @param index The position in the array.
 * @return Pointer to the User object.
 */
User* UserDynamicArray::getPtr(int index) {
    return &data[index];
}

/**
 * Adds a new user to the end of the array.
 * Automatically triggers a resize if the capacity is reached.
 * @param u The user object to be added.
 */
void UserDynamicArray::add(const User& u) {
    if (count >= capacity) resize();
    data[count] = u;
    count++;
}

/**
 * Searches for a user's index based on their unique UserID.
 * @param userID The string ID to search for.
 * @return The integer index if found, -1 otherwise.
 */
int UserDynamicArray::findIndexByUserID(const string& userID) const {
    for (int i = 0; i < count; i++) {
        if (data[i].getUserID() == userID) {
            return i;
        }
    }
    return -1;
}

/**
 * Finds a user by their ID and returns a pointer to the object.
 * @param userID The string ID to search for.
 * @return Pointer to the found User, or nullptr if not found.
 */
User* UserDynamicArray::findByUserID(const string& userID) {
    int idx = findIndexByUserID(userID);
    if (idx == -1) return nullptr;
    return &data[idx];
}

/**
 * Removes a user from the array by their ID.
 * Shifts all subsequent elements left to maintain array contiguousness.
 * @param userID The string ID of the user to remove.
 * @return True if removal was successful, false if user not found.
 */
bool UserDynamicArray::removeByUserID(const string& userID) {
    int idx = findIndexByUserID(userID);
    if (idx == -1) return false;

    for (int i = idx; i < count - 1; i++) {
        data[i] = data[i + 1];
    }
    count--;
    return true;
}

/**
 * Iterates through the array and prints the details of every user.
 */
void UserDynamicArray::printAll() const {
    for (int i = 0; i < count; i++) {
        cout << "---------------------\n";
        data[i].print();
    }
}