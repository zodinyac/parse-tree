#include "Operator.h"

map<char, Operator> Operator::operators = {
    { '+', Operator('+', 1, Operator::Associativity::LEFT, Operator::Type::BINARY) },
    { '-', Operator('-', 1, Operator::Associativity::LEFT, Operator::Type::BINARY) },
    { '*', Operator('*', 2, Operator::Associativity::LEFT, Operator::Type::BINARY) },
    { '/', Operator('/', 2, Operator::Associativity::LEFT, Operator::Type::BINARY) },
    { '^', Operator('^', 3, Operator::Associativity::RIGHT, Operator::Type::BINARY) },
    { '!', Operator('!', 4, Operator::Associativity::RIGHT, Operator::Type::UNARY) },
    { '?', Operator('?', 5, Operator::Associativity::LEFT, Operator::Type::UNARY) }
};

Operator::Operator() : noop(true)
{
}

Operator::Operator(char op, int precedence, Operator::Associativity associativity, Operator::Type type) : noop(false),
                                                                                                          op(op),
                                                                                                          precedence(precedence),
                                                                                                          associativity(associativity),
                                                                                                          type(type)
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

bool Operator::is_unary()
{
    return !noop && type == Operator::Type::UNARY;
}

bool Operator::is_binary()
{
    return !noop && type == Operator::Type::BINARY;
}

Operator Operator::get_operator(char op)
{
    auto result = operators.find(op);
    if (result == operators.end()) {
        return Operator();
    }
    return result->second;
}
