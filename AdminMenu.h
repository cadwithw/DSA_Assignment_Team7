#ifndef ADMINMENU_H
#define ADMINMENU_H

#include "GameDynamicArray.h"
#include "UserDynamicArray.h"
#include "BorrowLinkedList.h"

class AdminMenu {
public:
    static void show(GameDynamicArray& games, UserDynamicArray& users, BorrowLinkedList& records);

private:
    // Helper logic functions
    static void handleAddGame(GameDynamicArray& games);
    static void handleRemoveGame(GameDynamicArray& games);
    static void handleAddMember(UserDynamicArray& users);
    static void handleGameSearch(GameDynamicArray& games);
    static void handleFilterAndSort(GameDynamicArray& games);

    // Utility for the menu
    static void displayGamesTable(GameDynamicArray& list);
};

#endif