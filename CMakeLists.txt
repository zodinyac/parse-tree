cmake_minimum_required(VERSION 3.6)
project(parse_tree)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
    src/main.cpp src/Node.cpp src/Node.h src/Token.cpp src/Token.h src/Parser.cpp src/Parser.h src/Operator.cpp src/Operator.h src/TokenKind.h src/OperatorKind.h src/OperatorAssociativity.h src/OperatorType.h)

add_executable(parse_tree ${SOURCE_FILES})
