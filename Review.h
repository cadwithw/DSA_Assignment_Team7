struct ReviewNode {
    string gameID;
    string memberName;
    string comment;
    int rating;
    ReviewNode* nextInBucket; // For Chaining collisions

    ReviewNode(string gID, string name, string msg, int r)
        : gameID(gID), memberName(name), comment(msg), rating(r), nextInBucket(nullptr) {
    }
};