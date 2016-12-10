#ifndef PARSE_TREE_TOKEN_H
#define PARSE_TREE_TOKEN_H

#include <string>
#include <sstream>
using namespace std;

class Token
{
public:
    enum class Type {
        NONE,
        LEFTPAREN,
        RIGHTPAREN,
        UNOP,
        BINOP,
        OTHER
    };

    Token(Token::Type type = Token::Type::NONE);
    explicit Token(char op);
    explicit Token(string other);

    Token::Type get_type();
    char get_op();

    void print();

    operator string() const;

    static Token get_token();
    static void read_token(stringstream &ss);

private:
    static Token current_token;

    Token::Type type;
    char op;
    string other;
};


#endif //PARSE_TREE_TOKEN_H
