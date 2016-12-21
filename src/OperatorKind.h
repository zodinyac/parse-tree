#ifndef PARSE_TREE_OPERATORKIND_H
#define PARSE_TREE_OPERATORKIND_H

enum class OperatorKind {
    NOP,
#define UNARY_OPERATOR(Name, Spelling, Precedence, Associativity, Type) UO_##Name,
#define BINARY_OPERATOR(Name, Spelling, Precedence, Associativity) BO_##Name,
#define SPECIAL_OPERATOR(Name, Spelling, Precedence, Associativity) SO_##Name,
#   include "OperatorKind.def"
#undef SPECIAL_OPERATOR
#undef BINARY_OPERATOR
#undef UNARY_OPERATOR
};

#endif //PARSE_TREE_OPERATORKIND_H
