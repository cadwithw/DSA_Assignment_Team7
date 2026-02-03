#include "AllUsersMenu.h"
#include <iostream>

using namespace std;

void AllUsersMenu::show(GameDynamicArray& games) {
    cout << "[AllUsersMenu] Available Games\n";
    games.printAll();
}

void AllUsersMenu::displayGameDetails(GameDynamicArray& games) {
    cout << "[AllUsersMenu] Game Details\n";
}

void AllUsersMenu::listGamesByPlayers(GameDynamicArray& games) {
    cout << "[AllUsersMenu] Games by Players\n";
}
