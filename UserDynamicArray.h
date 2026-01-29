#ifndef USERDYNAMICARRAY_H
#define USERDYNAMICARRAY_H

#include "User.h"
#include <string>
using namespace std;

class UserDynamicArray {
private:
    User* data;
    int capacity;
    int count;

    void resize();

public:
    UserDynamicArray();
    ~UserDynamicArray();

    int size() const;
    User get(int index) const;
    User* getPtr(int index);

    void add(const User& u);

    int findIndexByUserID(const string& userID) const;
    User* findByUserID(const string& userID);

    bool removeByUserID(const string& userID);

    void printAll() const;
};

#endif
