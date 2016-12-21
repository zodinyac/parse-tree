#include "Token.h"
#include <cctype>
#include <iostream>
using namespace std;

Token Token::CurrentToken;
Token Token::NextToken;

Token::Token(TokenKind Kind) : Kind(Kind)
{
}

Token::Token(const Operator *Op) : Op(Op)
{
    if (Op->isUnary()) {
        Kind = TokenKind::UnOp;
    } else if (Op->isBinary()) {
        Kind = TokenKind::BinOp;
    } else if (Op->isSpecial()) {
        Kind = TokenKind::SpecialOp;
    } else {
        Kind = TokenKind::None;
    }
}

Token::Token(string Literal) : Literal(Literal)
{
    Kind = TokenKind::Literal;
}

TokenKind Token::getKind() const
{
    return Kind;
}

void Token::setKind(TokenKind Kind)
{
    this->Kind = Kind;
}

const Operator *Token::getOp() const
{
    return (Op ? Op : Operator::getOperatorNOP());
}

void Token::setOp(const Operator *Op)
{
    this->Op = Op;
}

bool Token::is(TokenKind Kind) const
{
    return this->Kind == Kind;
}

bool Token::isNot(TokenKind Kind) const
{
    return !is(Kind);
}

bool Token::isOneOf(TokenKind Kind1, TokenKind Kind2) const
{
    return is(Kind1) || is(Kind2);
}

template <typename... Ts>
bool Token::isOneOf(TokenKind Kind1, TokenKind Kind2, Ts... Kinds) const
{
    return is(Kind1) || isOneOf(Kind2, Kinds...);
}

bool Token::isLiteral() const
{
    return is(TokenKind::Literal);
}

bool Token::isAnyOp() const
{
    return isOneOf(TokenKind::UnOp, TokenKind::BinOp, TokenKind::SpecialOp);
}

bool Token::isNotOp() const
{
    return !isAnyOp();
}

bool Token::isSymbolicOp() const
{
    return isAnyOp() && isNot(TokenKind::SpecialOp);
}

Token::operator string() const
{
    switch (Kind) {
        default:
        case TokenKind::None:
            return "\n";

        case TokenKind::LeftParen:
            return "(";

        case TokenKind::RightParen:
            return ")";

        case TokenKind::Parens:
            return "()";

        case TokenKind::LeftBracket:
            return "[";

        case TokenKind::RightBracket:
            return "]";

        case TokenKind::Brackets:
            return "[]";

        case TokenKind::LeftBrace:
            return "{";

        case TokenKind::RightBrace:
            return "}";

        case TokenKind::Braces:
            return "{}";

        case TokenKind::UnOp:
        case TokenKind::BinOp:
            return Op->getSpelling();

        case TokenKind::SpecialOp:
            return Op->getSpellingShort();

        case TokenKind::Literal:
            return Literal;
    }
}

Token &Token::getCurrentToken()
{
    return CurrentToken;
}

void Token::readNextToken(bool OpMustBeUnary, stringstream &ss)
{
    if (NextToken.isNot(TokenKind::None)) {
        CurrentToken = NextToken;
        NextToken = Token();
        return;
    }

    char c;
    bool ok = static_cast<bool>(ss >> c);

    if (!ok) {
        CurrentToken = Token();
        return;
    }

    if (c == '(') {
        CurrentToken = Token(TokenKind::LeftParen);
        return;
    } else if (c == ')') {
        CurrentToken = Token(TokenKind::RightParen);
        return;
    } else if (c == '[') {
        CurrentToken = Token(TokenKind::LeftBracket);
        return;
    } else if (c == ']') {
        CurrentToken = Token(TokenKind::RightBracket);
        return;
    } else if (c == '{') {
        CurrentToken = Token(TokenKind::LeftBrace);
        return;
    } else if (c == '}') {
        CurrentToken = Token(TokenKind::RightBrace);
        return;
    } else if (c == '\'') {
        CurrentToken = Token(ReadChar(ss));
        return;
    } else if (c == '"') {
        CurrentToken = Token(ReadString(ss));
        return;
    }

    ss.unget();

    bool UnaryOpMustBePostfix = CurrentToken.isOneOf(TokenKind::RightParen, TokenKind::RightBracket, TokenKind::Literal);
    string Spelling;
    if (Operator::isOperatorSpellingSymbol(c)) {
        Spelling = ReadOperator(OpMustBeUnary, UnaryOpMustBePostfix, ss);
    }

    if (!Spelling.empty()) {
        const Operator *Op = Operator::findOperator(Spelling, OpMustBeUnary, UnaryOpMustBePostfix);
        if (Op->is(OperatorKind::BO_TernaryQuestion)) {
            CurrentToken = Token(Op);
            NextToken = Token(TokenKind::LeftParen);
        } else if (Op->is(OperatorKind::BO_TernaryColon)) {
            CurrentToken = Token(TokenKind::RightParen);
            NextToken = Token(Op);
        } else {
            CurrentToken = Token(Op);
        }
    } else {
        CurrentToken = Token(ReadLiteral(ss));
    }
}

string Token::ReadOperator(bool OpMustBeUnary, bool UnaryOpMustBePostfix, stringstream &ss)
{
    string op;
    bool IsFirst = true;
    bool IsAlpha = false;

    char c;
    bool ok;
    while ((ok = static_cast<bool>(ss >> noskipws >> c >> skipws))
           && ((!IsAlpha && Operator::isOperatorSpellingSymbol(c)) || (IsAlpha && (isalnum(c) || c == '_')))) {
        if (IsFirst) {
            IsFirst = false;
            if (isalpha(c) || c == '_') {
                IsAlpha = true;
            }
        }
        op.push_back(c);
    }

    if (ok) {
        ss.unget();
    } else {
        ss.clear();
    }

    IsAlpha = !IsFirst && IsAlpha && (Operator::findOperator(op, OpMustBeUnary, UnaryOpMustBePostfix) == nullptr);
    while (op.length() > 0 && (Operator::findOperator(op, OpMustBeUnary, UnaryOpMustBePostfix) == nullptr || IsAlpha)) {
        ss.unget();
        op.pop_back();
    }

    return op;
}

string Token::ReadChar(std::stringstream &ss)
{
    string Literal;

    char c;
    while (static_cast<bool>(ss >> noskipws >> c >> skipws) && (c != '\'')) {
        Literal.push_back(c);
    }

    if (Literal.empty()) {
        cerr << "Empty char literal." << endl;
        exit(2);
    }

    if (c == '\'') {
        return "\'" + Literal + "\'";
    } else {
        cerr << "Unmatched ' (char literal end symbol)." << endl;
        exit(2);
    }
}

string Token::ReadString(std::stringstream &ss)
{
    string Literal;

    char c;
    while (static_cast<bool>(ss >> noskipws >> c >> skipws) && (c != '\"')) {
        Literal.push_back(c);
    }

    if (c == '\"') {
        return "\"" + Literal + "\"";
    } else {
        cerr << "Unmatched \" (string literal end symbol)." << endl;
        exit(2);
    }
}

string Token::ReadLiteral(std::stringstream &ss)
{
    string Literal;

    char c;
    bool ok;
    while ((ok = static_cast<bool>(ss >> noskipws >> c >> skipws)) && ((c == '_') || isalnum(c))) {
        Literal.push_back(c);
    }

    if (ok) {
        ss.unget();
    } else {
        ss.clear();
    }

    return Literal;
}
