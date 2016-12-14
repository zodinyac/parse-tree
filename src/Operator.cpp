#include "Operator.h"
using namespace std;

#define P(x) (16 - (x))
/* http://en.cppreference.com/w/c/language/operator_precedence */
vector<Operator> Operator::operators = {
        /* Suffix/postfix increment and decrement */
        Operator("++", "POSTFIX_INCREMENT", P(1), Operator::Associativity::LEFT, Operator::Type::UNARY_POSTFIX),
        Operator("--", "POSTFIX_DECREMENT", P(1), Operator::Associativity::LEFT, Operator::Type::UNARY_POSTFIX),

        /* Function call () */
        Operator("function_call", "FUNCTION_CALL", P(1), Operator::Associativity::LEFT, Operator::Type::SPECIAL),

        /* Array subscribing [] */
        Operator("array_subscribing", "ARRAY_SUBSCRIBING", P(1), Operator::Associativity::LEFT, Operator::Type::SPECIAL),

        /* Structure and union member access */
        Operator(".", "MEMBER_ACCESS", P(1), Operator::Associativity::LEFT, Operator::Type::BINARY),

        /* Structure and union member access through pointer */
        Operator("->", "MEMBER_ACCESS_POINTER", P(1), Operator::Associativity::LEFT, Operator::Type::BINARY),

        /* Compound literal (type){list} */
        Operator("compound_literal", "COMPOUND_LITERAL", P(1), Operator::Associativity::LEFT, Operator::Type::SPECIAL),


        /* Prefix increment and decrement */
        Operator("++", "PREFIX_INCREMENT", P(2), Operator::Associativity::RIGHT, Operator::Type::UNARY),
        Operator("--", "PREFIX_DECREMENT", P(2), Operator::Associativity::RIGHT, Operator::Type::UNARY),

        /* Unary plus and minus */
        Operator("+", "UNARY_PLUS", P(2), Operator::Associativity::RIGHT, Operator::Type::UNARY),
        Operator("-", "UNARY_MINUS", P(2), Operator::Associativity::RIGHT, Operator::Type::UNARY),

        /* Logical NOT and bitwise NOT */
        Operator("!", "LOGICAL_NOT", P(2), Operator::Associativity::RIGHT, Operator::Type::UNARY),
        Operator("~", "BITWISE_NOT", P(2), Operator::Associativity::RIGHT, Operator::Type::UNARY),

        /* Type cast (type) */
        Operator("type_cast", "TYPE_CAST", P(2), Operator::Associativity::RIGHT, Operator::Type::SPECIAL),

        /* Indirection (dereference) */
        Operator("*", "DEREFERENCE", P(2), Operator::Associativity::RIGHT, Operator::Type::UNARY),

        /* Address-of */
        Operator("&", "ADDRESS_OF", P(2), Operator::Associativity::RIGHT, Operator::Type::UNARY),

        /* Size-of */
        Operator("sizeof", "SIZE_OF", P(2), Operator::Associativity::RIGHT, Operator::Type::UNARY),

        /* Alignment requirement */
        Operator("_Alignof", "ALIGNMENT", P(2), Operator::Associativity::RIGHT, Operator::Type::UNARY),


        /* Multiplication, division, and remainder */
        Operator("*", "MULTIPLICATION", P(3), Operator::Associativity::LEFT, Operator::Type::BINARY),
        Operator("/", "DIVISION", P(3), Operator::Associativity::LEFT, Operator::Type::BINARY),
        Operator("%", "REMAINDER", P(3), Operator::Associativity::LEFT, Operator::Type::BINARY),


        /* Addition and subtraction */
        Operator("+", "ADDICTION", P(4), Operator::Associativity::LEFT, Operator::Type::BINARY),
        Operator("-", "SUBTRACTION", P(4), Operator::Associativity::LEFT, Operator::Type::BINARY),


        /* Bitwise left shift and right shift */
        Operator("<<", "BITWISE_LEFT_SHIFT", P(5), Operator::Associativity::LEFT, Operator::Type::BINARY),
        Operator(">>", "BITWISE_RIGHT_SHIFT", P(5), Operator::Associativity::LEFT, Operator::Type::BINARY),


        /* For relational operators < and ≤ respectively */
        Operator("<", "LESS", P(6), Operator::Associativity::LEFT, Operator::Type::BINARY),
        Operator("<=", "LESS_EQUAL", P(6), Operator::Associativity::LEFT, Operator::Type::BINARY),

        /* For relational operators > and ≥ respectively */
        Operator(">", "GREATER", P(6), Operator::Associativity::LEFT, Operator::Type::BINARY),
        Operator(">=", "GREATER_EQUAL", P(6), Operator::Associativity::LEFT, Operator::Type::BINARY),


        /* For relational = and ≠ respectively */
        Operator("==", "EQUAL", P(7), Operator::Associativity::LEFT, Operator::Type::BINARY),
        Operator("!=", "NOT_EUAL", P(7), Operator::Associativity::LEFT, Operator::Type::BINARY),


        /* Bitwise AND */
        Operator("&", "BITWISE_AND", P(8), Operator::Associativity::LEFT, Operator::Type::BINARY),


        /* Bitwise XOR (exclusive or) */
        Operator("^", "BITWISE_XOR", P(9), Operator::Associativity::LEFT, Operator::Type::BINARY),


        /* Bitwise OR (inclusive or) */
        Operator("|", "BITWISE_OR", P(10), Operator::Associativity::LEFT, Operator::Type::BINARY),


        /* Logical AND */
        Operator("&&", "LOGICAL_AND", P(11), Operator::Associativity::LEFT, Operator::Type::BINARY),


        /* Logical OR */
        Operator("||", "LOGICAL_OR", P(12), Operator::Associativity::LEFT, Operator::Type::BINARY),

        /* Ternary conditional ?: */
        Operator("?", "TERNARY_?", P(13), Operator::Associativity::LEFT, Operator::Type::BINARY),
        Operator(":", "TERNARY_:", P(13), Operator::Associativity::LEFT, Operator::Type::BINARY),


        /* Simple assignment */
        Operator("=", "ASSIGNMENT", P(14), Operator::Associativity::RIGHT, Operator::Type::BINARY),

        /* Assignment by sum and difference */
        Operator("+=", "ASSIGNMENT_SUM", P(14), Operator::Associativity::RIGHT, Operator::Type::BINARY),
        Operator("-=", "ASSIGNMENT_DIFFERENCE", P(14), Operator::Associativity::RIGHT, Operator::Type::BINARY),

        /* Assignment by product, quotient, and remainder */
        Operator("*=", "ASSIGNMENT_PRODUCT", P(14), Operator::Associativity::RIGHT, Operator::Type::BINARY),
        Operator("/=", "ASSIGNMENT_QUOTIENT", P(14), Operator::Associativity::RIGHT, Operator::Type::BINARY),
        Operator("%=", "ASSIGNMENT_REMAINDER", P(14), Operator::Associativity::RIGHT, Operator::Type::BINARY),

        /* Assignment by bitwise left shift and right shift */
        Operator("<<=", "ASSIGNMENT_BITWISE_LEFT_SHIFT", P(14), Operator::Associativity::RIGHT, Operator::Type::BINARY),
        Operator(">>=", "ASSIGNMENT_BITWISE_RIGHT_SHIFT", P(14), Operator::Associativity::RIGHT, Operator::Type::BINARY),

        /* Assignment by bitwise AND, XOR, and OR */
        Operator("&=", "ASSIGNMENT_BITWISE_AND", P(14), Operator::Associativity::RIGHT, Operator::Type::BINARY),
        Operator("^=", "ASSIGNMENT_BITWISE_XOR", P(14), Operator::Associativity::RIGHT, Operator::Type::BINARY),
        Operator("|=", "ASSIGNMENT_BITWISE_OR", P(14), Operator::Associativity::RIGHT, Operator::Type::BINARY),


        /* Comma */
        Operator(",", "COMMA", P(15), Operator::Associativity::LEFT, Operator::Type::BINARY)
};
#undef P

Operator::Operator() : noop(true)
{
}

Operator::Operator(string op, string id, int precedence, Operator::Associativity associativity,
                   Operator::Type type) : noop(false),
                                          op(op),
                                          id(id),
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
    return noop || type == Operator::Type::SPECIAL;
}

string Operator::get_op() const
{
    return op;
}

string Operator::get_op_short() const
{
    string short_name = string() + op.front();
    size_t found = 0;
    while ((found = op.find("_", found)) != string::npos) {
        found++;
        if (found < op.length()) {
            short_name += op[found];
        }
    }
    return short_name;
}

string Operator::get_id() const
{
    return id;
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
    return isOp() && (type == Operator::Type::UNARY || type == Operator::Type::UNARY_POSTFIX);
}

bool Operator::is_unary_prefix() const
{
    return isOp() && type == Operator::Type::UNARY;
}

bool Operator::is_unary_postfix() const
{
    return isOp() && type == Operator::Type::UNARY_POSTFIX;
}

bool Operator::is_binary() const
{
    return isOp() && type == Operator::Type::BINARY;
}

Operator Operator::get_operator(bool all, bool postfix, string op)
{
    for (auto op_obj: operators) {
        if (op_obj.isOp()) {
            if (op_obj.get_op() == op) {
                if (all) {
                    if (op_obj.is_unary()) {
                        if (postfix && op_obj.is_unary_postfix()) {
                            return op_obj;
                        } else if (!postfix && op_obj.is_unary_prefix()) {
                            return op_obj;
                        }
                    } else {
                        return op_obj;
                    }
                } else if (!all && op_obj.is_unary()) {
                    if (postfix && op_obj.is_unary_postfix()) {
                        return op_obj;
                    } else if (!postfix && op_obj.is_unary_prefix()) {
                        return op_obj;
                    }
                }
            }
        }
    }

    return Operator();
}

Operator Operator::get_operator_by_id(string id)
{
    for (auto op_obj: operators) {
        if (op_obj.get_id() == id) {
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
