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

char Token::get_op()
{
    return op;
}

void Token::print()
{
    switch (type) {
        default:
        case Token::Type::NONE:
            cout << endl;
            break;

        case Token::Type::LEFTPAREN:
            cout << "(";
            break;

        case Token::Type::RIGHTPAREN:
            cout << ")";
            break;

        case Token::Type::BINOP:
            cout << op;
            break;

        case Token::Type::OTHER:
            cout << other;
            break;
    }
}

Token Token::get_token()
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
