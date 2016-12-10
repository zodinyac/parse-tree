#include <cctype>
#include <iostream>
#include "Operator.h"
#include "Token.h"
using namespace std;

Token Token::current_token;

Token::Token(Token::Type type) : type(type)
{
}

Token::Token(Operator op) : op(op)
{
    if (op.is_unary()) {
        type = Token::Type::UNOP;
    } else {
        type = Token::Type::BINOP;
    }
}

Token::Token(string other) : other(other)
{
    type = Token::Type::OTHER;
}

Token::Type Token::get_type() const
{
    return type;
}

void Token::set_type(Token::Type type)
{
    this->type = type;
}

Operator Token::get_op() const
{
    return op;
}

void Token::print()
{
    cout << string(*this);
}

Token::operator string() const
{
    switch (type) {
        default:
        case Token::Type::NONE:
            return "\n";

        case Token::Type::LEFTPAREN:
            return "(";

        case Token::Type::RIGHTPAREN:
            return ")";

        case Token::Type::PARENS:
            return "()";

        case Token::Type::UNOP:
        case Token::Type::BINOP:
            return op.get_op();

        case Token::Type::OTHER:
            return other;
    }
}

Token &Token::get_token()
{
    return current_token;
}

void Token::read_token(bool all, stringstream &ss)
{
    char c;
    bool ok = static_cast<bool>(ss >> c);

    if (!ok) {
        current_token = Token();
        return;
    }

    if (c == '(') {
        current_token = Token(Token::Type::LEFTPAREN);
        return;
    } else if (c == ')') {
        current_token = Token(Token::Type::RIGHTPAREN);
        return;
    }


    ss.unget();

    string op;
    if (Operator::is_operator_symbol(c)) {
        op = read_operator(all, ss);

    }

    if (!op.empty()) {
        current_token = Token(Operator::get_operator(all, op));
    } else {
        current_token = Token(read_other(ss));
    }
}

string Token::read_operator(bool all, stringstream &ss)
{
    string op;
    bool is_alpha = false;

    char c;
    bool ok;
    while ((ok = static_cast<bool>(ss >> noskipws >> c >> skipws)) && Operator::is_operator_symbol(c)) {
        if (isalpha(c) || c == '_') {
            is_alpha = true;
        }
        op.push_back(c);
    }

    if (ok) {
        ss.unget();
    } else {
        ss.clear();
    }

    is_alpha = is_alpha && Operator::get_operator(all, op).isNoOp();
    while (op.length() > 0 && (Operator::get_operator(all, op).isNoOp() || is_alpha)) {
        ss.unget();
        op.pop_back();
    }

    return op;
}

string Token::read_other(std::stringstream &ss)
{
    string other;
    Token::NumberType type = Token::NumberType::UNKNOWN;

    char c;
    bool ok;
    while ((ok = static_cast<bool>(ss >> noskipws >> c >> skipws)) && ((c == '_') || isalnum(c) || (type == Token::NumberType::NUMBER && c == '.'))) {
        if (type == Token::NumberType::UNKNOWN) {
            if (isdigit(c)) {
                type = Token::NumberType::NUMBER;
            } else {
                type = Token::NumberType::NOT_NUMBER;
            }
        }
        other.push_back(c);
    }

    if (ok) {
        ss.unget();
    } else {
        ss.clear();
    }

    return other;
}
