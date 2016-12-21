#ifndef PARSE_TREE_NODE_H
#define PARSE_TREE_NODE_H

#include "Token.h"
#include <deque>
#include <string>

class Node {
public:
    Node(Token Tok, Node *Left = nullptr, Node *Right = nullptr);

    const Token *getToken() const;
    void setToken(Token Tok);

    Node *getLeft();
    void setLeft(Node *Left = nullptr);

    Node *getRight();
    void setRight(Node *Right);

    void print(bool Pretty = false) const;

    operator std::string() const;

private:
    Token Tok;
    Node *Left;
    Node *Right;

    void printInternal() const;
    void printPrettyInternal(int Level, int IndentSpace) const;

    int maxHeight(const Node *Node) const;
    void printBranches(int BranchLen, int NodeSpaceLen, int StartLen, int NodesInThisLevel, const std::deque<const Node *> &NodesQueue) const;
    void printNodes(int BranchLen, int NodeSpaceLen, int StartLen, int NodesInThisLevel, const std::deque<const Node *> &NodesQueue) const;
    void printLeaves(int IndentSpace, int Level, int NodesInThisLevel, const std::deque<const Node *> &NodesQueue) const;
};


#endif //PARSE_TREE_NODE_H
