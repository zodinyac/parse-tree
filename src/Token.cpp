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
    type = Token::Type::BINOP;
}

Token::Token(string other) : other(other)
{
    type = Token::Type::OTHER;
}

Token::Type Token::get_type()
{
    return type;
}

void Token::set_type(Token::Type type)
{
    this->type = type;
}

Operator Token::get_op()
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
    } else if (c == ')') {
        current_token = Token(Token::Type::RIGHTPAREN);
    } else if (Operator::is_operator_symbol(c)) {
        ss.unget();
        string op = read_operator(all, ss);
        current_token = Token(Operator::get_operator(all, op));
    } else {
        ss.unget();
        current_token = Token(read_other(ss));
    }
}

string Token::read_operator(bool all, stringstream &ss)
{
    string op;

    char c;
    bool ok;
    while ((ok = static_cast<bool>(ss >> noskipws >> c >> skipws)) && Operator::is_operator_symbol(c)) {
        op.push_back(c);
    }

    if (ok) {
        ss.unget();
    }

    while (op.length() > 0 && Operator::get_operator(all, op).isNoOp()) {
        ss.putback(op.back());
        op.pop_back();
    }

    return op;
}

string Token::read_other(std::stringstream &ss)
{
    string other;

    char c;
    bool ok;
    while ((ok = static_cast<bool>(ss >> c)) && ((c == '_') || isalnum(c))) {
        other.push_back(c);
    }

    if (ok) {
        ss.unget();
    }

    return other;
}
