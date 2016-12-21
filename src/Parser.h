#ifndef PARSE_TREE_PARSER_H
#define PARSE_TREE_PARSER_H

#include "Node.h"
#include <string>
#include <sstream>

class Parser
{
public:
    Parser(std::string Expression);

    Node *parse();

private:
    Node *parseAtom();
    Node *parseExpression(int MinPrecedence);

    std::stringstream ss;
};


#endif //PARSE_TREE_PARSER_H
