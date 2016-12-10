#include "Node.h"

Node::Node(Token token, Node *left, Node *right) : token(token), left(left), right(right)
{
}

const Token *Node::getToken()
{
    return &token;
}

void Node::print() {
    if (left) {
        left->print();
    }
    token.print();
    if (right) {
        right->print();
    }
}
