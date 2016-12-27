#ifndef PARSE_TREE_OPERATOR_H
#define PARSE_TREE_OPERATOR_H

#include "OperatorAssociativity.h"
#include "OperatorKind.h"
#include "OperatorType.h"
#include <string>
#include <vector>

class Operator
{
public:
    Operator();
    Operator(OperatorKind Kind, std::string Spelling, int Precedence,
             OperatorAssociativity Associativity, OperatorType Type);

    OperatorKind getKind() const;
    bool is(OperatorKind Kind) const;
    bool isNot(OperatorKind Kind) const;
    bool isOneOf(OperatorKind Kind1, OperatorKind Kind2) const;
    template <typename... Ts>
    bool isOneOf(OperatorKind Kind1, OperatorKind Kind2, Ts... Kinds) const
    {
        return is(Kind1) || isOneOf(Kind2, Kinds...);
    }

    std::string getSpelling() const;
    std::string getSpellingShort() const;

    int getPrecedence() const;

    OperatorAssociativity getAssociativity() const;
    bool isLeftAssociativity() const;
    bool isRightAssociativity() const;

    OperatorType getType() const;
    bool isUnary() const;
    bool isUnaryPrefix() const;
    bool isUnaryPostfix() const;
    bool isBinary() const;
    bool isSpecial() const;

    static const Operator *findOperator(std::string Spelling, bool OpMustBeOnlyUnary, bool UnaryOpMustBePostfix);
    static const Operator *getOperatorByKind(OperatorKind Kind);
    static const Operator *getOperatorNOP();

    static bool isOperatorSpellingSymbol(char c);

private:
    OperatorKind Kind = OperatorKind::NOP;
    std::string Spelling;
    int Precedence = 0;
    OperatorAssociativity Associativity = OperatorAssociativity::Left;
    OperatorType Type = OperatorType::Special;

    static std::vector<Operator> Operators;
};


#endif //PARSE_TREE_OPERATOR_H
