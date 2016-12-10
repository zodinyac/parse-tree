#include <iostream>
using namespace std;

#include "Operator.h"
#include "Parser.h"
#include "Token.h"

Parser::Parser(string expression)
{
    ss.str(expression);
}

Node *Parser::parse()
{
    cout << "Parse expression: " << ss.str() << endl;

    Token::read_token(false, ss);
    return parse_expression(1);
}

Node *Parser::parse_atom()
{
    Token token = Token::get_token();

    if (token.get_type() == Token::Type::NONE) {
        return nullptr;
        //cerr << "Source ended unexpectedly." << endl;
        //exit(2);
    }

    if (token.get_type() == Token::Type::LEFTPAREN) {
        Token::read_token(false, ss);
        Node *node = parse_expression(1);
        if (Token::get_token().get_type() != Token::Type::RIGHTPAREN) {
            cerr << "Unmatched '('." << endl;
            exit(2);
        }
        Token::read_token(true, ss);
        return new Node(Token::Type::PARENS, node);
    }

    if (token.get_type() == Token::Type::BINOP) {
        if (token.get_op().is_unary()) {
            Token::get_token().set_type(Token::Type::UNOP);
            return nullptr;
        }
        return nullptr;
        //cerr << "Expected an atom, not an operator '" << token.get_op() << "'." << endl;
        //exit(2);
    }

    Token::read_token(true, ss);
    return new Node(token);
}

Node *Parser::parse_expression(int min_prec)
{
    Node *atom_lhs = parse_atom();
    while (true) {
        Token token = Token::get_token();
        if (token.get_type() == Token::Type::NONE
            || (token.get_type() != Token::Type::UNOP && token.get_type() != Token::Type::BINOP)) {
            break;
        }

        Operator op = token.get_op();
        if (op.get_precedence() < min_prec) {
            break;
        }

        Token::read_token(true, ss);
        int next_min_prec = op.get_precedence() + (op.get_associativity() == Operator::Associativity::LEFT);
        Node *atom_rhs = parse_expression(next_min_prec);
        atom_lhs = new Node(token, atom_lhs, atom_rhs);
    }

    return atom_lhs;
}
