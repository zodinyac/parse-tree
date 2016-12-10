#include "Operator.h"
using namespace std;

#define P(x) (16 - (x))
/* http://en.cppreference.com/w/c/language/operator_precedence */
vector<Operator> Operator::operators = {
        Operator("++", P(1), Operator::Associativity::LEFT, Operator::Type::UNARY),
        /* Function call () */
        /* Array subscribing [] */
        Operator(".", P(1), Operator::Associativity::LEFT, Operator::Type::BINARY),
        Operator("->", P(1), Operator::Associativity::LEFT, Operator::Type::BINARY),
        /* Compound literal (type){list} */

        Operator("++", P(2), Operator::Associativity::RIGHT, Operator::Type::UNARY),
        Operator("--", P(2), Operator::Associativity::RIGHT, Operator::Type::UNARY),
        Operator("!", P(2), Operator::Associativity::RIGHT, Operator::Type::UNARY),
        Operator("~", P(2), Operator::Associativity::RIGHT, Operator::Type::UNARY),
        /* Type cast (type) */
        Operator("type_cast", P(2), Operator::Associativity::RIGHT, Operator::Type::NOOP),
        Operator("*", P(2), Operator::Associativity::RIGHT, Operator::Type::UNARY),
        Operator("&", P(2), Operator::Associativity::RIGHT, Operator::Type::UNARY),
        Operator("sizeof", P(2), Operator::Associativity::RIGHT, Operator::Type::UNARY),
        Operator("_Alignof", P(2), Operator::Associativity::RIGHT, Operator::Type::UNARY),

        Operator("*", P(3), Operator::Associativity::LEFT, Operator::Type::BINARY),
        Operator("/", P(3), Operator::Associativity::LEFT, Operator::Type::BINARY),
        Operator("%", P(3), Operator::Associativity::LEFT, Operator::Type::BINARY),

        Operator("+", P(4), Operator::Associativity::LEFT, Operator::Type::BINARY),
        Operator("-", P(4), Operator::Associativity::LEFT, Operator::Type::BINARY),

        Operator("<<", P(5), Operator::Associativity::LEFT, Operator::Type::BINARY),
        Operator(">>", P(5), Operator::Associativity::LEFT, Operator::Type::BINARY),

        Operator("<", P(6), Operator::Associativity::LEFT, Operator::Type::BINARY),
        Operator("<=", P(6), Operator::Associativity::LEFT, Operator::Type::BINARY),
        Operator(">", P(6), Operator::Associativity::LEFT, Operator::Type::BINARY),
        Operator(">=", P(6), Operator::Associativity::LEFT, Operator::Type::BINARY),

        Operator("==", P(7), Operator::Associativity::LEFT, Operator::Type::BINARY),
        Operator("!=", P(7), Operator::Associativity::LEFT, Operator::Type::BINARY),

        Operator("&", P(8), Operator::Associativity::LEFT, Operator::Type::BINARY),

        Operator("^", P(9), Operator::Associativity::LEFT, Operator::Type::BINARY),

        Operator("|", P(10), Operator::Associativity::LEFT, Operator::Type::BINARY),

        Operator("&&", P(11), Operator::Associativity::LEFT, Operator::Type::BINARY),

        Operator("||", P(12), Operator::Associativity::LEFT, Operator::Type::BINARY),

        /* Ternary conditional ?: */

        Operator("=", P(14), Operator::Associativity::RIGHT, Operator::Type::BINARY),
        Operator("+=", P(14), Operator::Associativity::RIGHT, Operator::Type::BINARY),
        Operator("-=", P(14), Operator::Associativity::RIGHT, Operator::Type::BINARY),
        Operator("*=", P(14), Operator::Associativity::RIGHT, Operator::Type::BINARY),
        Operator("/=", P(14), Operator::Associativity::RIGHT, Operator::Type::BINARY),
        Operator("%=", P(14), Operator::Associativity::RIGHT, Operator::Type::BINARY),
        Operator("<<=", P(14), Operator::Associativity::RIGHT, Operator::Type::BINARY),
        Operator(">>=", P(14), Operator::Associativity::RIGHT, Operator::Type::BINARY),
        Operator("&=", P(14), Operator::Associativity::RIGHT, Operator::Type::BINARY),
        Operator("^=", P(14), Operator::Associativity::RIGHT, Operator::Type::BINARY),
        Operator("|=", P(14), Operator::Associativity::RIGHT, Operator::Type::BINARY),

        Operator(",", P(15), Operator::Associativity::LEFT, Operator::Type::BINARY)
};
#undef P

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
    return !isNoOp();
}

bool Operator::isNoOp() const
{
    return noop || type == Operator::Type::NOOP;
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

Operator::Type Operator::get_type() const
{
    return type;
}

bool Operator::is_unary() const
{
    return isOp() && type == Operator::Type::UNARY;
}

bool Operator::is_binary() const
{
    return isOp() && type == Operator::Type::BINARY;
}

Operator Operator::get_operator(bool all, string op)
{
    for (auto op_obj: operators) {
        if (op_obj.isOp()) {
            if (op_obj.get_op() == op) {
                if (all) {
                    return op_obj;
                } else if (!all && op_obj.is_unary()) {
                    return op_obj;
                }
            }
        }
    }

    return Operator();
}

Operator Operator::get_operator_by_name(string op)
{
    for (auto op_obj: operators) {
        if (op_obj.get_op() == op) {
            return op_obj;
        }
    }

    return Operator();
}

bool Operator::is_operator_symbol(char c)
{
    static string symbols;
    if (symbols.empty()) {
        for (auto op: operators) {
            if (op.isOp()) {
                for (auto cop: op.get_op()) {
                    if (symbols.find(cop) == symbols.npos) {
                        symbols.push_back(cop);
                    }
                }
            }
        }
    }

    return symbols.find(c) != symbols.npos;
}
