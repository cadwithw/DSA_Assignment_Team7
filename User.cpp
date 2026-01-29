#include "User.h"
#include <iostream>
using namespace std;

User::User() {
    userID = "";
    name = "";
    role = MEMBER;
}

User::User(string id, string n, Role r) {
    userID = id;
    name = n;
    role = r;
}

string User::getUserID() const { return userID; }
string User::getName() const { return name; }
Role User::getRole() const { return role; }

bool User::isAdmin() const { return role == ADMIN; }
bool User::isMember() const { return role == MEMBER; }

void User::print() const {
    cout << "User ID: " << userID << "\n";
    cout << "Name: " << name << "\n";
    cout << "Role: " << (role == ADMIN ? "ADMIN" : "MEMBER") << "\n";
}
