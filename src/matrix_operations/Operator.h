#pragma once

enum class Operator{
    PLUS,
    MINUS,
    ASSIGN,
    MUL,
    TRANSPOSE,
    UNITE,
    CUT,
    INV,
    DET,
    RANK,
    GAUSS,
    IMPORT,
    EXPORT,
    PRINT,
    SCAN,
    LEFT_PARENTHESIS,
    RIGHT_PARENTHESIS,
    MATRIX_BRACE_RIGHT,
    MATRIX_BRACE_LEFT
};

enum class OperatorPriority{
    PLUS = 2,
    MINUS = 2,
    ASSIGN = 0,
    MUL = 3,
    TRANSPOSE = 1,
    UNITE = 1,
    CUT = 1,
    INV = 1,
    DET = 1,
    RANK = 1,
    GAUSS = 1,
    IMPORT = 1,
    EXPORT = 1,
    PRINT = 1,
    SCAN = 1,
    LEFT_PARENTHESIS = 100,
    RIGHT_PARENTHESIS = 100,
    MATRIX_BRACE_RIGHT = 99,
    MATRIX_BRACE_LEFT = 99
};
