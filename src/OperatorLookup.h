#pragma once

#include <string>
#include <unordered_map>
#include "Operator.h"

inline const std::unordered_map<std::string, Operator> OPERATOR_LOOKUP {
    {"+", Operator::PLUS},
    {"-", Operator::MINUS},
    {"=", Operator::ASSIGN},
    {"*", Operator::MUL},
    {"UNITE", Operator::UNITE},
    {"CUT", Operator::CUT},
    {"TRANSPOSE", Operator::TRANSPOSE},
    {"INV", Operator::INV},
    {"DET", Operator::DET},
    {"RANK", Operator::RANK},
    {"GAUSS", Operator::GAUSS},
    {"IMPORT", Operator::IMPORT},
    {"EXPORT", Operator::EXPORT},
    {"PRINT", Operator::PRINT},
    {"SCAN", Operator::SCAN}
};

inline const std::unordered_map<Operator, std::size_t> N_OF_ARGS_LOOKUP {
    {Operator::PLUS, 2},
    {Operator::MINUS, 2},
    {Operator::ASSIGN, 2},
    {Operator::MUL, 2},
    {Operator::TRANSPOSE, 1},
    {Operator::UNITE, 2},
    {Operator::CUT, 5},
    {Operator::INV, 1},
    {Operator::DET, 1},
    {Operator::RANK, 1},
    {Operator::GAUSS, 1},
    {Operator::IMPORT, 1},
    {Operator::EXPORT, 1},
    {Operator::PRINT, 1},
    {Operator::SCAN, 1}
};

inline const std::unordered_map<std::string, OperatorPriority> PRIORITY_LOOKUP{
    {"+", OperatorPriority::PLUS},
    {"-", OperatorPriority::MINUS},
    {"=", OperatorPriority::ASSIGN},
    {"*", OperatorPriority::MUL},
    {"UNITE", OperatorPriority::UNITE},
    {"CUT", OperatorPriority::CUT},
    {"(", OperatorPriority::LEFT_PARENTHESIS},
    {")", OperatorPriority::RIGHT_PARENTHESIS},
    {"[", OperatorPriority::MATRIX_BRACE_LEFT},
    {"]", OperatorPriority::MATRIX_BRACE_RIGHT},
    {"TRANSPOSE", OperatorPriority::TRANSPOSE},
    {"INV", OperatorPriority::INV},
    {"DET", OperatorPriority::DET},
    {"RANK", OperatorPriority::RANK},
    {"GAUSS", OperatorPriority::GAUSS},
    {"IMPORT", OperatorPriority::IMPORT},
    {"EXPORT", OperatorPriority::EXPORT},
    {"PRINT", OperatorPriority::PRINT},
    {"SCAN", OperatorPriority::SCAN}};
