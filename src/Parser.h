#ifndef PARSE_TREE_PARSER_H
#define PARSE_TREE_PARSER_H

#include <string>
#include <sstream>
using namespace std;

#include "Node.h"

class Parser
{
public:
    Parser(string expression);

    Node *parse();

private:
    Node *parse_atom();
    Node *parse_expression(int min_prec);

    stringstream ss;
};


#endif //PARSE_TREE_PARSER_H
