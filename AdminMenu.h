#ifndef ADMINMENU_H
#define ADMINMENU_H

#include "GameDynamicArray.h"
#include "UserDynamicArray.h"
#include "BorrowLinkedList.h"

class AdminMenu {
public:
    static void show(GameDynamicArray& games, UserDynamicArray& users, BorrowLinkedList& records);
};

#endif
