#ifndef PARSE_TREE_OPERATOR_H
#define PARSE_TREE_OPERATOR_H

#include <string>
#include <vector>

class Operator
{
public:
    enum class Associativity {
        LEFT,
        RIGHT
    };
    enum class Type {
        UNARY,
        BINARY
    };

    Operator();
    Operator(std::string op, int precedence, Operator::Associativity associativity, Operator::Type type);

    bool isOp() const;
    bool isNoOp() const;

    std::string get_op() const;
    int get_precedence() const;
    Operator::Associativity get_associativity() const;

    bool is_unary() const;
    bool is_binary() const;

    static Operator get_operator(bool all, std::string op);
    static bool is_operator_symbol(char c);

private:
    static std::vector<Operator> operators;

    bool noop = true;

    std::string op;
    int precedence;
    Operator::Associativity associativity;
    Operator::Type type;
};


#endif //PARSE_TREE_OPERATOR_H
