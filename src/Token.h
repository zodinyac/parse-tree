#ifndef PARSE_TREE_TOKEN_H
#define PARSE_TREE_TOKEN_H

#include <string>
#include <sstream>
#include "Operator.h"

class Token
{
public:
    enum class Type {
        NONE,
        LEFTPAREN,
        RIGHTPAREN,
        PARENS,
        LEFTBRACKET,
        RIGHTBRACKET,
        BRACKETS,
        LEFTBRACE,
        RIGHTBRACE,
        BRACES,
        UNOP,
        BINOP,
        SPECIALOP,
        OTHER
    };

    Token(Token::Type type = Token::Type::NONE);
    explicit Token(Operator op);
    explicit Token(std::string other);

    Token::Type get_type() const;
    void set_type(Token::Type type);

    Operator get_op() const;

    void print();

    operator std::string() const;

    static Token &get_token();
    static void read_token(bool all, std::stringstream &ss);

private:
    enum NumberType {
        UNKNOWN,
        NOT_NUMBER,
        NUMBER
    };
    static std::string read_operator(bool all, bool postfix, std::stringstream &ss);
    static std::string read_char(std::stringstream &ss);
    static std::string read_string(std::stringstream &ss);
    static std::string read_other(std::stringstream &ss);

    static Token current_token;
    static Token next_token;
    Token::Type type;
    Operator op;
    std::string other;
};


#endif //PARSE_TREE_TOKEN_H
