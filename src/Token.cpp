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
    } else if (op.is_binary()) {
        type = Token::Type::BINOP;
    } else {
        type = Token::Type::SPECIALOP;
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

        case Token::Type::LEFTBRACKET:
            return "[";

        case Token::Type::RIGHTBRACKET:
            return "]";

        case Token::Type::BRACKETS:
            return "[]";

        case Token::Type::LEFTBRACE:
            return "{";

        case Token::Type::RIGHTBRACE:
            return "}";

        case Token::Type::BRACES:
            return "{}";

        case Token::Type::UNOP:
        case Token::Type::BINOP:
            return op.get_op();

        case Token::Type::SPECIALOP:
            return op.get_op_short();

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
    } else if (c == '[') {
        current_token = Token(Token::Type::LEFTBRACKET);
        return;
    } else if (c == ']') {
        current_token = Token(Token::Type::RIGHTBRACKET);
        return;
    } else if (c == '{') {
        current_token = Token(Token::Type::LEFTBRACE);
        return;
    } else if (c == '}') {
        current_token = Token(Token::Type::RIGHTBRACE);
        return;
    } else if (c == '\'') {
        current_token = Token(read_char(ss));
        return;
    } else if (c == '"') {
        current_token = Token(read_string(ss));
        return;
    }

    ss.unget();

    bool postfix = (current_token.get_type() == Token::Type::RIGHTPAREN
                   || current_token.get_type() == Token::Type::RIGHTBRACKET
                   || current_token.get_type() == Token::Type::OTHER);

    string op;
    if (Operator::is_operator_symbol(c)) {
        op = read_operator(all, postfix, ss);
    }

    if (!op.empty()) {
        current_token = Token(Operator::get_operator(all, postfix, op));
    } else {
        current_token = Token(read_other(ss));
    }
}

string Token::read_operator(bool all, bool postfix, stringstream &ss)
{
    string op;
    bool is_first = true;
    bool is_alpha = false;

    char c;
    bool ok;
    while ((ok = static_cast<bool>(ss >> noskipws >> c >> skipws))
           && ((!is_alpha && Operator::is_operator_symbol(c)) || (is_alpha && (isalnum(c) || c == '_')))) {
        if (is_first) {
            is_first = false;
            if (isalpha(c) || c == '_') {
                is_alpha = true;
            }
        }
        op.push_back(c);
    }

    if (ok) {
        ss.unget();
    } else {
        ss.clear();
    }

    is_alpha = !is_first && is_alpha && Operator::get_operator(all, postfix, op).isNoOp();
    while (op.length() > 0 && (Operator::get_operator(all, postfix, op).isNoOp() || is_alpha)) {
        ss.unget();
        op.pop_back();
    }

    return op;
}

string Token::read_char(std::stringstream &ss)
{
    string other;

    char c;
    while (static_cast<bool>(ss >> noskipws >> c >> skipws) && (c != '\'')) {
        other.push_back(c);
    }

    if (other.empty()) {
        cerr << "Empty char literal." << endl;
        exit(2);
    }

    if (c == '\'') {
        return "\'" + other + "\'";
    } else {
        cerr << "Unmatched ' (char literal end symbol)." << endl;
        exit(2);
    }
}

string Token::read_string(std::stringstream &ss)
{
    string other;

    char c;
    while (static_cast<bool>(ss >> noskipws >> c >> skipws) && (c != '\"')) {
        other.push_back(c);
    }

    if (c == '\"') {
        return "\"" + other + "\"";
    } else {
        cerr << "Unmatched \" (string literal end symbol)." << endl;
        exit(2);
    }
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
