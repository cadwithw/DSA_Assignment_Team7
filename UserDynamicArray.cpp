#include "UserDynamicArray.h"
#include <iostream>
using namespace std;

UserDynamicArray::UserDynamicArray() {
    capacity = 10;
    count = 0;
    data = new User[capacity];
}

UserDynamicArray::~UserDynamicArray() {
    delete[] data;
}

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

int UserDynamicArray::size() const {
    return count;
}

User UserDynamicArray::get(int index) const {
    return data[index];
}

User* UserDynamicArray::getPtr(int index) {
    return &data[index];
}

void UserDynamicArray::add(const User& u) {
    if (count >= capacity) resize();
    data[count] = u;
    count++;
}

int UserDynamicArray::findIndexByUserID(const string& userID) const {
    for (int i = 0; i < count; i++) {
        if (data[i].getUserID() == userID) {
            return i;
        }
    }
    return -1;
}

User* UserDynamicArray::findByUserID(const string& userID) {
    int idx = findIndexByUserID(userID);
    if (idx == -1) return nullptr;
    return &data[idx];
}

bool UserDynamicArray::removeByUserID(const string& userID) {
    int idx = findIndexByUserID(userID);
    if (idx == -1) return false;

    for (int i = idx; i < count - 1; i++) {
        data[i] = data[i + 1];
    }
    count--;
    return true;
}

void UserDynamicArray::printAll() const {
    for (int i = 0; i < count; i++) {
        cout << "---------------------\n";
        data[i].print();
    }
}
