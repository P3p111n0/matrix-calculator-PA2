#include "MatrixFactory.h"
#include "DenseMatrix.h"
#include "SparseMatrix.h"
#include <vector>

MatrixFactory::MatrixFactory(double ratio) : _ratio(ratio) {}

MatrixMemoryRepr * MatrixFactory::get_initial_repr(std::size_t rows,
                                                   std::size_t columns) const {
    return new SparseMatrix(rows, columns);
}

template <typename T> bool is_sparse(const T & mx, double ratio) {
    std::size_t rows = mx.size();
    std::size_t columns = rows ? mx.begin()->size() : 0;
    double zeroes_to_be_sparse = rows * columns * ratio;

    std::size_t zero_count = 0;
    for (const auto & row : mx) {
        for (const auto & val : row) {
            if (val == 0) {
                ++zero_count;
            }
            if (zero_count >= zeroes_to_be_sparse) {
                return true;
            }
        }
    }
    return false;
}

MatrixMemoryRepr * MatrixFactory::get_initial_repr(
    std::initializer_list<std::initializer_list<double>> initializer) const {
    bool matrix_is_sparse = is_sparse(initializer, _ratio);

    if (matrix_is_sparse) {
        return new SparseMatrix(initializer);
    }
    return new DenseMatrix(initializer);
}

MatrixMemoryRepr * MatrixFactory::get_initial_repr(double val) const {
    SparseMatrix * repr = new SparseMatrix(1, 1);
    repr->modify(0, 0, val);
    return repr;
}

MatrixMemoryRepr * MatrixFactory::get_initial_repr(IteratorWrapper begin,
                                                   IteratorWrapper end) const {
    std::size_t distance = begin.distance(end);
    std::size_t number_of_non_zeroes = (1 - _ratio) * begin.get_matrix_rows() * begin.get_matrix_columns();

    if (distance < number_of_non_zeroes){
        return new SparseMatrix(std::move(begin), std::move(end));
    }
    return new DenseMatrix(std::move(begin), std::move(end));
}

MatrixMemoryRepr * MatrixFactory::convert(MatrixMemoryRepr * mx) const {
    if (mx->is_efficient(_ratio)) {
        return mx;
    }
    std::size_t non_zero_values = mx->begin().distance(mx->end());
    std::size_t ratio_to_be_dense = (1 - _ratio) * mx->rows() * mx->columns();
    if (non_zero_values <= ratio_to_be_dense) {
        return new SparseMatrix(mx->begin(), mx->end());
    }
    return new DenseMatrix(mx->begin(), mx->end());
}

double MatrixFactory::ratio() const { return _ratio; }
