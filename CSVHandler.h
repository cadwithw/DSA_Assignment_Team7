#ifndef CSVHANDLER_H
#define CSVHANDLER_H

#include <string>
using namespace std;

#include "GameDynamicArray.h"
#include "UserDynamicArray.h"
#include "BorrowLinkedList.h"

class CSVHandler {
public:
    static bool loadGames(const string& filename, GameDynamicArray& games);
    static bool saveGames(const string& filename, GameDynamicArray& games);

    static bool loadUsers(const string& filename, UserDynamicArray& users);
    static bool saveUsers(const string& filename, UserDynamicArray& users);

    static bool loadBorrowRecords(const string& filename, BorrowLinkedList& records);
    static bool saveBorrowRecords(const string& filename, BorrowLinkedList& records);
    static bool loadReviews(const string& filename, GameDynamicArray& games);
    static bool saveReviews(const string& filename, GameDynamicArray& games);
};

#endif
