#ifndef PARSE_TREE_TOKENKIND_H_H
#define PARSE_TREE_TOKENKIND_H_H

enum class TokenKind {
    None,
    LeftParen,
    RightParen,
    Parens,
    LeftBracket,
    RightBracket,
    Brackets,
    DoubleBrackets,
    LeftBrace,
    RightBrace,
    Braces,
    UnOp,
    BinOp,
    SpecialOp,
    Literal
};

#endif //PARSE_TREE_TOKENKIND_H_H
