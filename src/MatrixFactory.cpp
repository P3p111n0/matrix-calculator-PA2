#include "MatrixFactory.h"
#include "DenseMatrix.h"
#include "SparseMatrix.h"

MatrixMemoryRepr * MatrixFactory::get_initial_repr(std::size_t rows,
                                                   std::size_t columns) const {
    return new SparseMatrix(rows, columns);
}

MatrixMemoryRepr * MatrixFactory::get_initial_repr(
    std::initializer_list<std::initializer_list<double>> initializer) const {

    std::size_t rows = initializer.size();
    std::size_t columns = rows ? initializer.begin()->size() : 0;
    double zeroes_to_be_sparse = rows * columns * ratio;

    std::size_t zero_count = 0;
    for (const auto & row : initializer){
        for (const auto & val : row){
            if (val == 0) {
                ++zero_count;
            }
            if (zero_count >= zeroes_to_be_sparse){
                break;
            }
        }
    }
    if (zero_count >= zeroes_to_be_sparse){
        return new SparseMatrix(initializer);
    }
    return new DenseMatrix(initializer);
}

MatrixMemoryRepr * MatrixFactory::convert(MatrixMemoryRepr * mx) const {
    if (mx->is_efficient(ratio)){
        return mx;
    }
    std::size_t non_zero_values = mx->begin().distance(mx->end());
    std::size_t ratio_to_be_dense = (1 - ratio) * mx->rows() * mx->columns();
    if (non_zero_values <= ratio_to_be_dense){
        return new SparseMatrix(mx->begin(), mx->end());
    }
    return new DenseMatrix(mx->begin(), mx->end());
}
