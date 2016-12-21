#include "Node.h"
#include <cmath>
#include <iomanip>
#include <iostream>
using namespace std;

Node::Node(Token Tok, Node *Left, Node *Right) : Tok(Tok), Left(Left), Right(Right)
{
}

const Token *Node::getToken() const
{
    return &Tok;
}

void Node::setToken(Token Tok)
{
    this->Tok = Tok;
}

Node *Node::getLeft()
{
    return Left;
}

void Node::setLeft(Node *Left)
{
    this->Left = Left;
}

Node *Node::getRight()
{
    return Right;
}

void Node::setRight(Node *Right)
{
    this->Right = Right;
}

void Node::print(bool Pretty) const
{
    if (Pretty) {
        printPrettyInternal(1, 0);
    } else {
        printInternal();
    }
}

Node::operator string() const
{
    return Tok;
}

void Node::printInternal() const
{
    if (Tok.is(TokenKind::Parens)) {
        cout << "(";
        if (Left) {
            Left->printInternal();
        }
        cout << ")";
    } else if (Tok.is(TokenKind::Brackets)) {
        cout << "[";
        if (Left) {
            Left->printInternal();
        }
        cout << "]";
    } else if (Tok.is(TokenKind::Braces)) {
        cout << "{";
        if (Left) {
            Left->printInternal();
        }
        cout << "}";
    } else {
        if (Left) {
            Left->printInternal();
        }
        if (Tok.isSymbolicOp() || Tok.isLiteral()) {
            if (Tok.is(TokenKind::BinOp)) {
                cout << " ";
            }
            cout << string(Tok);
            if (Tok.is(TokenKind::BinOp)) {
                cout << " ";
            }
        }
        if (Right) {
            Right->printInternal();
        }
    }
}

/* http://articles.leetcode.com/how-to-pretty-print-binary-tree/ */
int Node::maxHeight(const Node *Node) const
{
    if (!Node) {
        return 0;
    }

    int LeftHeight = maxHeight(Node->Left);
    int RightHeight = maxHeight(Node->Right);

    return LeftHeight > RightHeight ? LeftHeight + 1 : RightHeight + 1;
}

void Node::printBranches(int BranchLen, int NodeSpaceLen, int StartLen, int NodesInThisLevel, const std::deque<const Node *> &NodesQueue) const
{
    auto iter = NodesQueue.cbegin();
    for (int i = 0; i < NodesInThisLevel / 2; i++) {
        cout << ((i == 0) ? setw(StartLen - 1) : setw(NodeSpaceLen - 2)) << "" << ((*iter++) ? "/" : " ");
        cout << setw(2 * BranchLen + 2) << "" << ((*iter++) ? "\\" : " ");
    }
    cout << endl;
}

void Node::printNodes(int BranchLen, int NodeSpaceLen, int StartLen, int NodesInThisLevel, const std::deque<const Node *> &NodesQueue) const
{
    auto iter = NodesQueue.cbegin();
    for (int i = 0; i < NodesInThisLevel; i++, iter++) {
        cout << ((i == 0) ? setw(StartLen) : setw(NodeSpaceLen)) << "" << ((*iter && (*iter)->Left) ? setfill('_') : setfill(' '));
        cout << setw(BranchLen + 2) << ((*iter) ? string(**iter) : "");
        cout << ((*iter && (*iter)->Right) ? setfill('_') : setfill(' ')) << setw(BranchLen) << "" << setfill(' ');
    }
    cout << endl;
}

void Node::printLeaves(int IndentSpace, int Level, int NodesInThisLevel, const std::deque<const Node *> &NodesQueue) const
{
    auto iter = NodesQueue.cbegin();
    for (int i = 0; i < NodesInThisLevel; i++, iter++) {
        cout << ((i == 0) ? setw(IndentSpace + 2) : setw(2 * Level + 2)) << ((*iter) ? string(**iter) : "");
    }
    cout << endl;
}

void Node::printPrettyInternal(int Level, int IndentSpace) const
{
    int h = maxHeight(this);
    int NodesInThisLevel = 1;

    int BranchLen = 2 * (static_cast<int>(pow(2.0, h)) - 1) - (3 - Level) * static_cast<int>(pow(2.0, h - 1));
    int NodeSpaceLen = 2 + (Level + 1) * static_cast<int>(pow(2.0, h));
    int StartLen = BranchLen + (3 - Level) + IndentSpace;

    deque<const Node *> NodesQueue;
    NodesQueue.push_back(this);
    for (int r = 1; r < h; r++) {
        printBranches(BranchLen, NodeSpaceLen, StartLen, NodesInThisLevel, NodesQueue);
        BranchLen = BranchLen / 2 - 1;
        NodeSpaceLen = NodeSpaceLen / 2 + 1;
        StartLen = BranchLen + (3 - Level) + IndentSpace;
        printNodes(BranchLen, NodeSpaceLen, StartLen, NodesInThisLevel, NodesQueue);

        for (int i = 0; i < NodesInThisLevel; i++) {
            const Node *CurrentNode = NodesQueue.front();
            NodesQueue.pop_front();
            if (CurrentNode) {
                NodesQueue.push_back(CurrentNode->Left);
                NodesQueue.push_back(CurrentNode->Right);
            } else {
                NodesQueue.push_back(nullptr);
                NodesQueue.push_back(nullptr);
            }
        }
        NodesInThisLevel *= 2;
    }
    printBranches(BranchLen, NodeSpaceLen, StartLen, NodesInThisLevel, NodesQueue);
    printLeaves(IndentSpace, Level, NodesInThisLevel, NodesQueue);
}
