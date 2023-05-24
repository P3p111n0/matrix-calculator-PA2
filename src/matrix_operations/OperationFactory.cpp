#include "OperationFactory.h"
#include "Operator.h"
#include "five_args/MatrixOpCut.h"
#include "single_argument/MatrixOpDet.h"
#include "single_argument/MatrixOpGauss.h"
#include "single_argument/MatrixOpInv.h"
#include "single_argument/MatrixOpRank.h"
#include "single_argument/MatrixOpTranspose.h"
#include "two_args/MatrixOpMinus.h"
#include "two_args/MatrixOpMul.h"
#include "two_args/MatrixOpPlus.h"
#include "two_args/MatrixOpUnite.h"
#include <stdexcept>

bool OperationCmp::operator()(const std::shared_ptr<MatrixOp> & lhs,
                              const std::shared_ptr<MatrixOp> & rhs) const {
    return lhs->name() < rhs->name();
}

inline const std::unordered_map<std::string, Operator> lookup_map = {
    {"+", Operator::PLUS},
    {"-", Operator::MINUS},
    {"*", Operator::MUL},
    {"UNITE", Operator::UNITE},
    {"CUT", Operator::CUT},
    {"TRANSPOSE", Operator::TRANSPOSE},
    {"INV", Operator::INV},
    {"DET", Operator::DET},
    {"RANK", Operator::RANK},
    {"GAUSS", Operator::GAUSS},
};

OperationFactory::OperationFactory() : _operations(lookup_map) {}

MatrixOp * OperationFactory::get_operation(const std::string & op_name) const {
    if (!_operations.count(op_name)){
        //this shouldn't happen
        throw std::invalid_argument("Unknown operation name: " + op_name);
    }
    Operator op = _operations.at(op_name);
    switch (op) {
    case Operator::PLUS:
        return new MatrixOpPlus;
    case Operator::MINUS:
        return new MatrixOpMinus;
    case Operator::MUL:
        return new MatrixOpMul;
    case Operator::TRANSPOSE:
        return new MatrixOpTranspose;
    case Operator::UNITE:
        return new MatrixOpUnite;
    case Operator::CUT:
        return new MatrixOpCut;
    case Operator::INV:
        return new MatrixOpInv;
    case Operator::DET:
        return new MatrixOpDet;
    case Operator::RANK:
        return new MatrixOpRank;
    case Operator::GAUSS:
        return new MatrixOpGauss;
    default:
        //this definitely shouldn't happen
        throw std::runtime_error("No matching operator for: " + op_name);
    }
}

std::set<std::shared_ptr<MatrixOp>, OperationCmp>
OperationFactory::get_all_operations() const {
    std::set<std::shared_ptr<MatrixOp>, OperationCmp> op_set;
    for (const auto & [key, op] : _operations){
        op_set.emplace(get_operation(key));
    }
    return op_set;
}

bool OperationFactory::is_operation(const std::string & name) const {
    return _operations.count(name);
}
