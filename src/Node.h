#ifndef PARSE_TREE_NODE_H
#define PARSE_TREE_NODE_H

#include "Token.h"

class Node {
public:
    Node(Token token, Node *left = nullptr, Node *right = nullptr);

    const Token *getToken();

    void print();

private:
    Token token;
    Node *left;
    Node *right;
};


#endif //PARSE_TREE_NODE_H
