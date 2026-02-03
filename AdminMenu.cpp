#include "AdminMenu.h"
#include <iostream>

using namespace std;

void AdminMenu::show(GameDynamicArray& games, UserDynamicArray& users, BorrowLinkedList& records) {
    cout << "[AdminMenu] Admin Features\n";
    games.printAll();
}
