#ifndef PARSE_TREE_TOKEN_H
#define PARSE_TREE_TOKEN_H

#include "Operator.h"
#include "TokenKind.h"
#include <string>
#include <sstream>

class Token
{
public:
    Token(TokenKind Kind = TokenKind::None);
    explicit Token(const Operator *Op);
    explicit Token(std::string Literal);

    TokenKind getKind() const;
    void setKind(TokenKind Kind);

    const Operator *getOp() const;
    void setOp(const Operator *Op);

    bool is(TokenKind Kind) const;
    bool isNot(TokenKind Kind) const;
    bool isOneOf(TokenKind Kind1, TokenKind Kind2) const;
    template <typename... Ts>
    bool isOneOf(TokenKind Kind1, TokenKind Kind2, Ts... Kinds) const;

    bool isLiteral() const;

    bool isAnyOp() const;
    bool isNotOp() const;
    bool isSymbolicOp() const;

    operator std::string() const;

    static Token &getCurrentToken();
    static void readNextToken(bool OpMustBeOnlyUnary, std::stringstream &ss);

private:
    TokenKind Kind;
    const Operator *Op = nullptr;
    std::string Literal;

    static Token CurrentToken;
    static Token NextToken;

    static std::string ReadOperator(bool OpMustBeOnlyUnary, bool UnaryOpMustBePostfix, std::stringstream &ss);

    static std::string ReadChar(std::stringstream &ss);
    static std::string ReadString(std::stringstream &ss);
    static std::string ReadLiteral(std::stringstream &ss);
};


#endif //PARSE_TREE_TOKEN_H
