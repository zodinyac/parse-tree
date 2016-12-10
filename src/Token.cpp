#include <iostream>
using namespace std;

#include "Operator.h"
#include "Token.h"

Token Token::current_token;

Token::Token(Token::Type type) : type(type)
{
}

Token::Token(char op) : op(op)
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

char Token::get_op()
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
            return string(1, op);

        case Token::Type::BINOP:
            return string(1, op);

        case Token::Type::OTHER:
            return other;
    }
}

Token &Token::get_token()
{
    return current_token;
}

void Token::read_token(stringstream &ss)
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
    } else if (Operator::get_operator(c).isOp()) {
        current_token = Token(c);
    } else {
        ss.unget();

        string value;
        ss >> value;

        current_token = Token(value);
    }
}
