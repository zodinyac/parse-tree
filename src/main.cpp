#include <iostream>
#include <string>
using namespace std;

#include "Node.h"
#include "Parser.h"

int main(int argc, char *argv[])
{
    if (argc != 2) {
        cerr << R"(Usage ./parse-tree <"expression">)";
        return 1;
    }

    Parser parser(argv[1]);
    Node *root = parser.parse();

    root->print_pretty();
}
