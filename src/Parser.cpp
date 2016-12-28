#include "Operator.h"
#include "Parser.h"
#include "Token.h"
#include <iostream>
using namespace std;

Parser::Parser(string Expression)
{
    ss.str(Expression);
}

Node *Parser::parse()
{
    cout << "Parse expression: " << ss.str() << endl;

    Token::readNextToken(true, ss);
    return parseExpression(1);
}

Node *Parser::parseAtom()
{
    Token Tok = Token::getCurrentToken();

    if (Tok.isOneOf(TokenKind::None, TokenKind::RightParen, TokenKind::RightBracket, TokenKind::RightBrace) || Tok.isAnyOp()) {
        return nullptr;
    }

    if (Tok.is(TokenKind::LeftParen)) {
        Token::readNextToken(true, ss);
        Node *node = parseExpression(1);
        if (Token::getCurrentToken().isNot(TokenKind::RightParen)) {
            cerr << "Unmatched '('." << endl;
            exit(2);
        }
        Token::readNextToken(false, ss);
        return new Node(TokenKind::Parens, node);
    }
    if (Tok.is(TokenKind::LeftBracket)) {
        Token::readNextToken(true, ss);
        if (Token::getCurrentToken().is(TokenKind::LeftBracket)) {
            Token::readNextToken(true, ss);
            Node *node = parseExpression(1);
            if (Token::getCurrentToken().isNot(TokenKind::RightBracket)) {
                cerr << "Unmatched '[['." << endl;
                exit(2);
            } else {
                Token::readNextToken(true, ss);
                if (Token::getCurrentToken().isNot(TokenKind::RightBracket)) {
                    cerr << "Unmatched '[['." << endl;
                    exit(2);
                }
            }
            Token::readNextToken(false, ss);
            return new Node(TokenKind::DoubleBrackets, node);
        } else {
            Node *node = parseExpression(1);
            if (Token::getCurrentToken().isNot(TokenKind::RightBracket)) {
                cerr << "Unmatched '['." << endl;
                exit(2);
            }
            Token::readNextToken(false, ss);
            return new Node(TokenKind::Brackets, node);
        }
    }
    if (Tok.is(TokenKind::LeftBrace)) {
        Token::readNextToken(true, ss);
        Node *node = parseExpression(1);
        if (Token::getCurrentToken().isNot(TokenKind::RightBrace)) {
            cerr << "Unmatched '{'." << endl;
            exit(2);
        }
        Token::readNextToken(false, ss);
        return new Node(TokenKind::Braces, node);
    }

    Token::readNextToken(false, ss);
    return new Node(Tok);
}

Node *Parser::parseExpression(int MinPrecedence)
{
    Node *Atom_LHS = parseAtom();
    while (true) {
        Token Tok = Token::getCurrentToken();

        if (Atom_LHS) {
            if (Atom_LHS->getToken()->is(TokenKind::Parens)) {
                if (Atom_LHS->getLeft()
                    && Atom_LHS->getLeft()->getToken()->getOp()->is(OperatorKind::UO_Deref)
                    && Tok.is(TokenKind::LeftParen)) {
                    Tok = Token(Operator::getOperatorByKind(OperatorKind::SO_FuncCall));
                } else if (Tok.is(TokenKind::LeftBrace)) {
                    Tok = Token(Operator::getOperatorByKind(OperatorKind::SO_CompoundLiteral));
                } else if (Tok.isOneOf(TokenKind::LeftParen, TokenKind::UnOp, TokenKind::Literal)) {
                    Tok = Token(Operator::getOperatorByKind(OperatorKind::SO_TypeCast));
                }
            } else if ((Atom_LHS->getToken()->is(TokenKind::Literal)
                        || Atom_LHS->getToken()->getOp()->isOneOf(OperatorKind::SO_ArraySubscribing, OperatorKind::SO_GetBits))
                       && Tok.is(TokenKind::LeftBracket)) {
                Tok = Token(Operator::getOperatorByKind(OperatorKind::SO_ArraySubscribing));
            } else if ((Atom_LHS->getToken()->is(TokenKind::Literal)
                        || Atom_LHS->getToken()->getOp()->getPrecedence() == Operator::getOperatorByKind(OperatorKind::SO_FuncCall)->getPrecedence())
                       && Tok.is(TokenKind::LeftParen)) {
                Tok = Token(Operator::getOperatorByKind(OperatorKind::SO_FuncCall));
            } else if (Atom_LHS->getToken()->is(TokenKind::Literal) && Tok.is(TokenKind::Literal)) {
                Tok = Token(Operator::getOperatorByKind(OperatorKind::SO_ConcatenateLiteral));
            }
        }

        if (Tok.is(TokenKind::None) || Tok.isNotOp()) {
            break;
        }

        const Operator *Op = Tok.getOp();
        if (Op->getPrecedence() < MinPrecedence) {
            break;
        }

        if (Tok.isSymbolicOp()) {
            Token::readNextToken(false, ss);
        }
        int NextMinPrecedence = Op->getPrecedence() + Op->isLeftAssociativity();
        Node *Atom_RHS = parseExpression(NextMinPrecedence);
        if (Atom_RHS && Atom_RHS->getToken()->is(TokenKind::DoubleBrackets)) {
            Tok = Token(Operator::getOperatorByKind(OperatorKind::SO_GetBits));
        }
        Atom_LHS = new Node(Tok, Atom_LHS, Atom_RHS);
    }

    return Atom_LHS;
}
