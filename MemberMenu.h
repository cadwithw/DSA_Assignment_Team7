#ifndef MEMBERMENU_H
#define MEMBERMENU_H

#include "User.h"
#include "GameDynamicArray.h"
#include "BorrowLinkedList.h"

class MemberMenu {
public:
    static void show(const User& member, GameDynamicArray& games, BorrowLinkedList& records);
};

#endif
