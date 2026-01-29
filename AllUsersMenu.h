#ifndef ALLUSERSMENU_H
#define ALLUSERSMENU_H

#include "GameDynamicArray.h"

class AllUsersMenu {
public:
    static void show(GameDynamicArray& games);
    static void displayGameDetails(GameDynamicArray& games);
    static void listGamesByPlayers(GameDynamicArray& games);
};

#endif
