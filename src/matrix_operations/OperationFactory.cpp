#include "OperationFactory.h"
#include "MatrixOp.h"
#include "five_args/MatrixOpCut.h"
#include "single_argument/MatrixOpDet.h"
#include "single_argument/MatrixOpGauss.h"
#include "single_argument/MatrixOpInv.h"
#include "single_argument/MatrixOpRank.h"
#include "single_argument/MatrixOpTranspose.h"
#include "special_cases/MatrixOpAssign.h"
#include "special_cases/MatrixOpExport.h"
#include "special_cases/MatrixOpImport.h"
#include "special_cases/MatrixOpPrint.h"
#include "two_args/MatrixOpMinus.h"
#include "two_args/MatrixOpMul.h"
#include "two_args/MatrixOpPlus.h"
#include "two_args/MatrixOpUnite.h"
#include <memory>

bool OperationCmp::operator()(const std::shared_ptr<MatrixOp> & lhs,
                              const std::shared_ptr<MatrixOp> & rhs) const {
    return lhs->name() < rhs->name();
}

OperationFactory::OperationFactory() {
    _operations.emplace("+", new MatrixOpPlus);
    _operations.emplace("-", new MatrixOpMinus);
    _operations.emplace("*", new MatrixOpMul);
    _operations.emplace("UNITE", new MatrixOpUnite);
    _operations.emplace("CUT", new MatrixOpCut);
    _operations.emplace("TRANSPOSE", new MatrixOpTranspose);
    _operations.emplace("INV", new MatrixOpInv);
    _operations.emplace("DET",new MatrixOpDet);
    _operations.emplace("RANK", new MatrixOpRank);
    _operations.emplace("GAUSS", new MatrixOpGauss);
    _operations.emplace("PRINT", new MatrixOpPrint);
    _operations.emplace("EXPORT", new MatrixOpExport);
    _operations.emplace("IMPORT", new MatrixOpImport);
    _operations.emplace("=", new MatrixOpAssign);

}

std::shared_ptr<MatrixOp>
OperationFactory::get_operation(const std::string & name) const {
    if (!_operations.count(name)) {
        // this shouldn't happen
        throw std::invalid_argument("Unknown operation name: " + name);
    }
    return _operations.at(name);
}

bool OperationFactory::is_operation(const std::string & name) const {
    return _operations.count(name);
}

std::size_t OperationFactory::priority_of(const std::string & op) const {
    std::shared_ptr<MatrixOp> operation(get_operation(op));
    return operation->priority();
}
