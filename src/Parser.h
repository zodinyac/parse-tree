#ifndef PARSE_TREE_PARSER_H
#define PARSE_TREE_PARSER_H

#include <string>
#include <sstream>
#include "Node.h"

class Parser
{
public:
    Parser(std::string expression);

    Node *parse();

private:
    Node *parse_atom();
    Node *parse_expression(int min_prec);

    std::stringstream ss;
};


#endif //PARSE_TREE_PARSER_H
