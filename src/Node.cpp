#include <cmath>
#include <iomanip>
#include <iostream>
using namespace std;

#include "Node.h"

Node::Node(Token token, Node *left, Node *right) : token(token), left(left), right(right)
{
}

const Token *Node::getToken() const
{
    return &token;
}

void Node::print()
{
    if (token.get_type() == Token::Type::PARENS) {
        cout << "(";
        if (left) {
            left->print();
        }
        cout << ")";
    } else {
        if (left) {
            left->print();
        }
        if (token.get_type() == Token::Type::OTHER || token.get_op().isOp()) {
            if (token.get_op().is_binary()) {
                cout << " ";
            }
            token.print();
            if (token.get_op().is_binary()) {
                cout << " ";
            }
        }
        if (right) {
            right->print();
        }
    }
}

void Node::print_pretty()
{
    print_pretty_internal(1, 0);
}

Node::operator string() const
{
    return token;
}

/* http://articles.leetcode.com/how-to-pretty-print-binary-tree/ */
int Node::max_height(Node *node)
{
    if (!node) {
        return 0;
    }

    int left_height = max_height(node->left);
    int right_height = max_height(node->right);

    return (left_height > right_height) ? left_height + 1: right_height + 1;
}

void Node::print_branches(int branch_len, int node_space_len, int start_len, int nodes_in_this_level, const deque<Node *> &nodes_queue)
{
    auto iter = nodes_queue.cbegin();
    for (int i = 0; i < nodes_in_this_level / 2; i++) {
        cout << ((i == 0) ? setw(start_len - 1) : setw(node_space_len - 2)) << "" << ((*iter++) ? "/" : " ");
        cout << setw(2 * branch_len + 2) << "" << ((*iter++) ? "\\" : " ");
    }
    cout << endl;
}

void Node::print_nodes(int branch_len, int node_space_len, int start_len, int nodes_in_this_level, const deque<Node *> &nodes_queue)
{
    auto iter = nodes_queue.cbegin();
    for (int i = 0; i < nodes_in_this_level; i++, iter++) {
        cout << ((i == 0) ? setw(start_len) : setw(node_space_len)) << "" << ((*iter && (*iter)->left) ? setfill('_') : setfill(' '));
        cout << setw(branch_len + 2) << ((*iter) ? string(**iter) : "");
        cout << ((*iter && (*iter)->right) ? setfill('_') : setfill(' ')) << setw(branch_len) << "" << setfill(' ');
    }
    cout << endl;
}

void Node::print_leaves(int indent_space, int level, int nodes_in_this_level, const deque<Node *> &nodes_queue)
{
    auto iter = nodes_queue.cbegin();
    for (int i = 0; i < nodes_in_this_level; i++, iter++) {
        cout << ((i == 0) ? setw(indent_space + 2) : setw(2 * level+2)) << ((*iter) ? string(**iter) : "");
    }
    cout << endl;
}

void Node::print_pretty_internal(int level, int indent_space)
{
    int h = max_height(this);
    int nodes_in_this_level = 1;

    int branch_len = 2 * (static_cast<int>(pow(2.0, h)) - 1) - (3 - level) * static_cast<int>(pow(2.0, h - 1));
    int node_space_len = 2 + (level + 1) * static_cast<int>(pow(2.0, h));
    int start_len = branch_len + (3 - level) + indent_space;

    deque<Node *> nodes_queue;
    nodes_queue.push_back(this);
    for (int r = 1; r < h; r++) {
        print_branches(branch_len, node_space_len, start_len, nodes_in_this_level, nodes_queue);
        branch_len = branch_len / 2 - 1;
        node_space_len = node_space_len / 2 + 1;
        start_len = branch_len + (3 - level) + indent_space;
        print_nodes(branch_len, node_space_len, start_len, nodes_in_this_level, nodes_queue);

        for (int i = 0; i < nodes_in_this_level; i++) {
            Node *curr_node = nodes_queue.front();
            nodes_queue.pop_front();
            if (curr_node) {
                nodes_queue.push_back(curr_node->left);
                nodes_queue.push_back(curr_node->right);
            } else {
                nodes_queue.push_back(nullptr);
                nodes_queue.push_back(nullptr);
            }
        }
        nodes_in_this_level *= 2;
    }
    print_branches(branch_len, node_space_len, start_len, nodes_in_this_level, nodes_queue);
    print_leaves(indent_space, level, nodes_in_this_level, nodes_queue);
}

