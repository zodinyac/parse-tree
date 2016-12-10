#include "Operator.h"

map<char, Operator> Operator::operators = {
    { '+', Operator('+', 1, Operator::Associativity::LEFT) },
    { '-', Operator('-', 1, Operator::Associativity::LEFT) },
    { '*', Operator('*', 2, Operator::Associativity::LEFT) },
    { '/', Operator('/', 2, Operator::Associativity::LEFT) },
    { '^', Operator('^', 3, Operator::Associativity::RIGHT) }
};

Operator::Operator() : noop(true)
{
}

Operator::Operator(char op, int precedence, Operator::Associativity associativity) : noop(false),
                                                                                     op(op),
                                                                                     precedence(precedence),
                                                                                     associativity(associativity)
{
}

bool Operator::isOp()
{
    return !noop;
}

bool Operator::isNoOp()
{
    return noop;
}

char Operator::get_op()
{
    return op;
}

int Operator::get_precedence()
{
    return precedence;
}

Operator::Associativity Operator::get_associativity()
{
    return associativity;
}

Operator Operator::get_operator(char op)
{
    auto result = operators.find(op);
    if (result == operators.end()) {
        return Operator();
    }
    return result->second;
}
