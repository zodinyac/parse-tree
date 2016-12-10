#include "Operator.h"
using namespace std;

vector<Operator> Operator::operators = {
    Operator("+", 1, Operator::Associativity::LEFT, Operator::Type::BINARY),
    Operator("-", 1, Operator::Associativity::LEFT, Operator::Type::BINARY),
    Operator("*", 2, Operator::Associativity::LEFT, Operator::Type::BINARY),
    Operator("/", 2, Operator::Associativity::LEFT, Operator::Type::BINARY),
    Operator("^", 3, Operator::Associativity::RIGHT, Operator::Type::BINARY),
    Operator("!", 4, Operator::Associativity::RIGHT, Operator::Type::UNARY),
    Operator("++", 4, Operator::Associativity::RIGHT, Operator::Type::UNARY),
    Operator("++", 5, Operator::Associativity::LEFT, Operator::Type::UNARY)
};

Operator::Operator() : noop(true)
{
}

Operator::Operator(string op, int precedence, Operator::Associativity associativity,
                   Operator::Type type) : noop(false),
                                          op(op),
                                          precedence(precedence),
                                          associativity(associativity),
                                          type(type)
{
}

bool Operator::isOp() const
{
    return !noop;
}

bool Operator::isNoOp() const
{
    return noop;
}

string Operator::get_op() const
{
    return op;
}

int Operator::get_precedence() const
{
    return precedence;
}

Operator::Associativity Operator::get_associativity() const
{
    return associativity;
}

bool Operator::is_unary() const
{
    return !noop && type == Operator::Type::UNARY;
}

bool Operator::is_binary() const
{
    return !noop && type == Operator::Type::BINARY;
}

Operator Operator::get_operator(bool all, string op)
{
    for (auto op_obj: operators) {
        if (op_obj.get_op() == op) {
            if (all) {
                return op_obj;
            } else if (!all && op_obj.is_unary()) {
                return op_obj;
            }
        }
    }

    return Operator();
}

bool Operator::is_operator_symbol(char c)
{
    static string symbols;
    if (symbols.empty()) {
        for (auto op: operators) {
            for (auto cop: op.get_op()) {
                if (symbols.find(cop) == symbols.npos) {
                    symbols.push_back(cop);
                }
            }
        }
    }

    return symbols.find(c) != symbols.npos;
}
