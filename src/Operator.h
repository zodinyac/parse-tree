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
        UNARY_POSTFIX,
        BINARY,
        SPECIAL
    };

    Operator();
    Operator(std::string op, int precedence = 0,
             Operator::Associativity associativity = Operator::Associativity::LEFT,
             Operator::Type type = Operator::Type::SPECIAL);

    bool isOp() const;
    bool isNoOp() const;

    std::string get_op() const;
    std::string get_op_short() const;
    int get_precedence() const;
    Operator::Associativity get_associativity() const;
    Operator::Type get_type() const;

    bool is_unary() const;
    bool is_unary_prefix() const;
    bool is_unary_postfix() const;
    bool is_binary() const;

    static Operator get_operator(bool all, bool postfix, std::string op);
    static Operator get_operator_by_name(std::string op);
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
