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

    Token::read_token(ss);
    return parse_expression(1);
}

Node *Parser::parse_atom()
{
    Token token = Token::get_token();

    if (token.get_type() == Token::Type::NONE) {
        cerr << "Source ended unexpectedly." << endl;
        exit(2);
    }

    if (token.get_type() == Token::Type::LEFTPAREN) {
        Token::read_token(ss);
        Node *node = parse_expression(1);
        if (Token::get_token().get_type() != Token::Type::RIGHTPAREN) {
            cerr << "Unmatched '('." << endl;
            exit(2);
        }
        Token::read_token(ss);
        return node;
    }

    if (token.get_type() == Token::Type::BINOP) {
        cerr << "Expected an atom, not an operator '" << token.get_op() << "'." << endl;
        exit(2);
    }

    Token::read_token(ss);
    return new Node(token);
}

Node *Parser::parse_expression(int min_prec)
{
    Node *atom_lhs = parse_atom();
    while (true) {
        Token token = Token::get_token();
        if (token.get_type() == Token::Type::NONE || token.get_type() != Token::Type::BINOP) {
            break;
        }

        Operator op = Operator::get_operator(token.get_op());
        if (op.get_precedence() < min_prec) {
            break;
        }

        Token::read_token(ss);
        int next_min_prec = op.get_precedence() + (op.get_associativity() == Operator::Associativity::LEFT);
        Node *atom_rhs = parse_expression(next_min_prec);

        atom_lhs = new Node(token, atom_lhs, atom_rhs);
    }

    return atom_lhs;
}
