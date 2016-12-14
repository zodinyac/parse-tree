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

    if (token.get_type() == Token::Type::NONE
        || token.get_type() == Token::Type::RIGHTPAREN
        || token.get_type() == Token::Type::RIGHTBRACKET
        || token.get_type() == Token::Type::UNOP
        || token.get_type() == Token::Type::BINOP
        || token.get_type() == Token::Type::SPECIALOP) {
        return nullptr;
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
    if (token.get_type() == Token::Type::LEFTBRACKET) {
        Token::read_token(false, ss);
        Node *node = parse_expression(1);
        if (Token::get_token().get_type() != Token::Type::RIGHTBRACKET) {
            cerr << "Unmatched '['." << endl;
            exit(2);
        }
        Token::read_token(true, ss);
        return new Node(Token::Type::BRACKETS, node);
    }
    if (token.get_type() == Token::Type::LEFTBRACE) {
        Token::read_token(false, ss);
        Node *node = parse_expression(1);
        if (Token::get_token().get_type() != Token::Type::RIGHTBRACE) {
            cerr << "Unmatched '{'." << endl;
            exit(2);
        }
        Token::read_token(true, ss);
        return new Node(Token::Type::BRACES, node);
    }

    Token::read_token(true, ss);
    return new Node(token);
}

Node *Parser::parse_expression(int min_prec)
{
    Node *atom_lhs = parse_atom();
    while (true) {
        Token token = Token::get_token();
        if (atom_lhs && atom_lhs->getToken()->get_type() == Token::Type::PARENS
            && (token.get_type() == Token::Type::OTHER
                || token.get_type() == Token::Type::LEFTPAREN
                || token.get_type() == Token::Type::UNOP)) {
            token = Token(Operator::get_operator_by_name("type_cast"));
        } else if (atom_lhs && atom_lhs->getToken()->get_type() == Token::Type::PARENS
                   && token.get_type() == Token::Type::LEFTBRACE) {
            token = Token(Operator::get_operator_by_name("compound_literal"));
        } else if (atom_lhs && (atom_lhs->getToken()->get_type() == Token::Type::OTHER
                                || (atom_lhs->getToken()->get_type() == Token::Type::SPECIALOP
                                    && atom_lhs->getToken()->get_op().get_op() == "array_subscribing"))
            && token.get_type() == Token::Type::LEFTPAREN) {
            token = Token(Operator::get_operator_by_name("function_call"));
        } else if (atom_lhs && (atom_lhs->getToken()->get_type() == Token::Type::OTHER
                                || (atom_lhs->getToken()->get_type() == Token::Type::SPECIALOP
                                    && atom_lhs->getToken()->get_op().get_op() == "array_subscribing"))
                   && token.get_type() == Token::Type::LEFTBRACKET) {
            token = Token(Operator::get_operator_by_name("array_subscribing"));
        }
        if (token.get_type() == Token::Type::NONE
            || (token.get_type() != Token::Type::UNOP
                && token.get_type() != Token::Type::BINOP
                && token.get_type() != Token::Type::SPECIALOP)) {
            break;
        }

        Operator op = token.get_op();
        if (op.get_precedence() < min_prec) {
            break;
        }

        if (token.get_op().isOp()) {
            Token::read_token(true, ss);
        }
        int next_min_prec = op.get_precedence() + (op.get_associativity() == Operator::Associativity::LEFT);
        Node *atom_rhs = parse_expression(next_min_prec);
        atom_lhs = new Node(token, atom_lhs, atom_rhs);
    }

    return atom_lhs;
}
