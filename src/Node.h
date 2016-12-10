#ifndef PARSE_TREE_NODE_H
#define PARSE_TREE_NODE_H

#include <deque>
#include <string>
#include "Token.h"

class Node {
public:
    Node(Token token, Node *left = nullptr, Node *right = nullptr);

    const Token *getToken() const;

    void print();
    void print_pretty();

    operator std::string() const;

private:
    int max_height(Node *node);

    void print_branches(int branch_len, int node_space_len, int start_len, int nodes_in_this_level, const std::deque<Node *> &nodes_queue);
    void print_nodes(int branch_len, int node_space_len, int start_len, int nodes_in_this_level, const std::deque<Node *> &nodes_queue);
    void print_leaves(int indent_space, int level, int nodes_in_this_level, const std::deque<Node *> &nodes_queue);
    void print_pretty_internal(int level, int indent_space);

    Token token;
    Node *left;
    Node *right;
};


#endif //PARSE_TREE_NODE_H
