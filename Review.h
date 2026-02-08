#ifndef REVIEW_H
#define REVIEW_H

#include <string> 

using namespace std;

/**
 * ReviewNode represents a single node in a Binary Search Tree (BST).
 * Each Game object will contain a root pointer to a tree of these nodes.
 */
struct ReviewNode {
    string gameID;
    string memberName;
    string comment;
    int rating;

    // BST Pointers: Left for lower ratings, Right for higher/equal ratings
    ReviewNode* left;
    ReviewNode* right;

    // Constructor to initialize a review
    ReviewNode(string gID, string name, string msg, int r)
        : gameID(gID), memberName(name), comment(msg), rating(r),
        left(nullptr), right(nullptr) {
    }
};

#endif