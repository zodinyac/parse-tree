#ifndef PARSE_TREE_OPERATOR_H
#define PARSE_TREE_OPERATOR_H

#include <map>
using namespace std;

class Operator
{
public:
    enum class Associativity {
        LEFT,
        RIGHT
    };

    Operator();
    Operator(char op, int precedence, Operator::Associativity associativity);

    bool isOp();
    bool isNoOp();

    char get_op();
    int get_precedence();
    Operator::Associativity get_associativity();

    static Operator get_operator(char op);

private:
    static map<char, Operator> operators;

    bool noop = true;

    char op;
    int precedence;
    Operator::Associativity associativity;
};


#endif //PARSE_TREE_OPERATOR_H
