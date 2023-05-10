#include "MatrixFactory.h"
#include "DenseMatrix.h"
#include "DenseMatrixIterator.h"
#include "IteratorWrapper.h"
#include "MatrixDimensions.h"
#include "SparseMatrix.h"
#include <vector>

struct MatrixSpoofer {
    MatrixSpoofer(MatrixDimensions dims,
                  const std::vector<std::vector<double>> & vec)
        : _dimensions(dims), _data(vec) {}
    MatrixSpoofer(const MatrixSpoofer &) = delete;
    MatrixDimensions _dimensions;
    const std::vector<std::vector<double>> & _data;
};

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

MatrixMemoryRepr *
MatrixFactory::get_initial_repr(const std::vector<std::vector<double>> & vec) const {
    bool matrix_is_sparse = is_sparse(vec, _ratio);

    std::size_t rows = vec.size();
    std::size_t columns = vec.empty() ? 0 : vec.begin()->size();

    MatrixSpoofer mx({rows, columns}, vec);
    DenseMatrix * spoofing_pointer = reinterpret_cast<DenseMatrix*>(&mx);
    IteratorWrapper begin(new DenseMatrixIterator(spoofing_pointer, 0, 0));
    IteratorWrapper end(new DenseMatrixIterator(spoofing_pointer, rows, 0));

    if (matrix_is_sparse){
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
