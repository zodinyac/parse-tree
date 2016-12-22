#include "Operator.h"
using namespace std;

vector<Operator> Operator::Operators = {
// TODO: calculate maximal precedence automatically instead of using 16
#define CALC_PRECEDENCE(Precedence) (16 - Precedence)
#define UNARY_OPERATOR(Name, Spelling, Precedence, Associativity, Type) \
        Operator(OperatorKind::UO_##Name, Spelling, CALC_PRECEDENCE(Precedence), OperatorAssociativity::Associativity, OperatorType::Unary##Type),
#define BINARY_OPERATOR(Name, Spelling, Precedence, Associativity) \
        Operator(OperatorKind::BO_##Name, Spelling, CALC_PRECEDENCE(Precedence), OperatorAssociativity::Associativity, OperatorType::Binary),
#define SPECIAL_OPERATOR(Name, Spelling, Precedence, Associativity) \
        Operator(OperatorKind::SO_##Name, Spelling, CALC_PRECEDENCE(Precedence), OperatorAssociativity::Associativity, OperatorType::Special),
#include "OperatorKind.def"
#undef SPECIAL_OPERATOR
#undef BINARY_OPERATOR
#undef UNARY_OPERATOR
#undef CALC_PRECEDENCE
};

Operator::Operator()
{
}

Operator::Operator(OperatorKind Kind, std::string Spelling, int Precedence,
                   OperatorAssociativity Associativity, OperatorType Type) : Kind(Kind),
                                                                             Spelling(Spelling),
                                                                             Precedence(Precedence),
                                                                             Associativity(Associativity),
                                                                             Type(Type)
{
}

OperatorKind Operator::getKind() const
{
    return Kind;
}

bool Operator::is(OperatorKind Kind) const
{
    return this->Kind == Kind;
}

bool Operator::isNot(OperatorKind Kind) const
{
    return !is(Kind);
}

bool Operator::isOneOf(OperatorKind Kind1, OperatorKind Kind2) const
{
    return is(Kind1) || is(Kind2);
}

template <typename... Ts>
bool Operator::isOneOf(OperatorKind Kind1, OperatorKind Kind2, Ts... Kinds) const
{
    return is(Kind1) || isOneOf(Kind2, Kinds...);
}

std::string Operator::getSpelling() const
{
    return Spelling;
}

string Operator::getSpellingShort() const
{
    string ShortName = string() + Spelling.front();
    size_t Found = 0;
    while ((Found = Spelling.find("_", Found)) != string::npos) {
        Found++;
        if (Found < Spelling.length()) {
            ShortName += Spelling[Found];
        }
    }
    return ShortName;
}

int Operator::getPrecedence() const
{
    return Precedence;
}

OperatorAssociativity Operator::getAssociativity() const
{
    return Associativity;
}

bool Operator::isLeftAssociativity() const
{
    return Associativity == OperatorAssociativity::Left;
}

bool Operator::isRightAssociativity() const
{
    return Associativity == OperatorAssociativity::Right;
}

OperatorType Operator::getType() const
{
    return Type;
}

bool Operator::isUnary() const
{
    return isUnaryPrefix() || isUnaryPostfix();
}

bool Operator::isUnaryPrefix() const
{
    return Type == OperatorType::UnaryPrefix;
}

bool Operator::isUnaryPostfix() const
{
    return Type == OperatorType::UnaryPostfix;
}

bool Operator::isBinary() const
{
    return Type == OperatorType::Binary;
}

bool Operator::isSpecial() const
{
    return Type == OperatorType::Special;
}

const Operator *Operator::findOperator(std::string Spelling, bool OpMustBeOnlyUnary, bool UnaryOpMustBePostfix)
{
    for (auto &Op: Operators) {
        if (Op.isSpecial()) {
            continue;
        }

        if (Op.getSpelling() != Spelling) {
            continue;
        }

        if (Op.isUnary()) {
            if (UnaryOpMustBePostfix && Op.isUnaryPostfix()) {
                return &Op;
            } else if (!UnaryOpMustBePostfix && Op.isUnaryPrefix()) {
                return &Op;
            }
        }

        if (!OpMustBeOnlyUnary && Op.isBinary()) {
            return &Op;
        }
    }

    return nullptr;
}

const Operator *Operator::getOperatorByKind(OperatorKind Kind)
{
    for (auto &Op: Operators) {
        if (Op.is(Kind)) {
            return &Op;
        }
    }

    return nullptr;
}

const Operator *Operator::getOperatorNOP()
{
    static Operator NOP;
    return &NOP;
}

bool Operator::isOperatorSpellingSymbol(char c)
{
    static string Symbols;
    if (Symbols.empty()) {
        for (auto &Op: Operators) {
            if (Op.isSpecial()) {
                continue;
            }

            for (auto SpellingSymbol: Op.Spelling) {
                if (Symbols.find(SpellingSymbol) == string::npos) {
                    Symbols.push_back(SpellingSymbol);
                }
            }
        }
    }

    return Symbols.find(c) != string::npos;
}
