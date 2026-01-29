#ifndef USER_H
#define USER_H

#include <string>
using namespace std;

enum Role {
    ADMIN = 0,
    MEMBER = 1
};

class User {
private:
    string userID;
    string name;
    Role role;

public:
    User();
    User(string id, string n, Role r);

    string getUserID() const;
    string getName() const;
    Role getRole() const;

    bool isAdmin() const;
    bool isMember() const;

    void print() const;
};

#endif
