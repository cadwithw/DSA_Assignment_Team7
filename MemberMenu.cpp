#include "MemberMenu.h"
#include <iostream>

using namespace std;

void MemberMenu::show(const User& member, GameDynamicArray& games, BorrowLinkedList& records) {
    cout << "\nMember Main Menu\n";
    cout << "1. Borrow a Game\n";
    cout << "2. Return a Game\n";
    cout << "3. Display Summary\n";
    cout << "4. Review Games\n";
    cout << "5. Record a Game\n";
    cout << "0. Return a Game\n";
    cout << "Choice: ";
}
