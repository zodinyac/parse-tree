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
    enum class Type {
        UNARY,
        BINARY
    };

    Operator();
    Operator(char op, int precedence, Operator::Associativity associativity, Operator::Type type);

    bool isOp();
    bool isNoOp();

    char get_op();
    int get_precedence();
    Operator::Associativity get_associativity();

    bool is_unary();
    bool is_binary();

    static Operator get_operator(char op);

private:
    static map<char, Operator> operators;

    bool noop = true;

    char op;
    int precedence;
    Operator::Associativity associativity;
    Operator::Type type;
};


#endif //PARSE_TREE_OPERATOR_H
