#pragma once

#include <string>
#include <unordered_map>
#include "Operator.h"

static const std::unordered_map<std::string, Operator> OPERATOR_LOOKUP{
    {"+", Operator::PLUS},
    {"-", Operator::MINUS},
    {"=", Operator::ASSIGN},
    {"*", Operator::MUL},
    {"UNITE", Operator::UNITE},
    {"CUT", Operator::CUT},
    {"(", Operator::LEFT_PARENTHESIS},
    {")", Operator::RIGHT_PARENTHESIS},
    {"[", Operator::MATRIX_BRACE_LEFT},
    {"]", Operator::MATRIX_BRACE_RIGHT}};

static const std::unordered_map<std::string, Operator> FUNCTION_LOOKUP{
    {"TRANSPOSE", Operator::TRANSPOSE},
    {"INV", Operator::INV},
    {"DET", Operator::DET},
    {"RANK", Operator::RANK},
    {"GAUSS", Operator::GAUSS},
    {"IMPORT", Operator::IMPORT},
    {"EXPORT", Operator::EXPORT},
    {"PRINT", Operator::PRINT},
    {"SCAN", Operator::SCAN}};

static const std::unordered_map<std::string, OperatorPriority> PRIORITY_LOOKUP{
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
