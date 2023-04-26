#include "SparseMatrix.h"
#include <algorithm>
#include <memory>
#include <queue>

bool SparseMatrix::MatrixElement::operator<(
    const SparseMatrix::MatrixElement & rhs) const {
    if (row != rhs.row) {
        return row < rhs.row;
    }
    return column < rhs.column;
}

SparseMatrix::SparseMatrix(size_t rows, size_t columns)
    : MatrixMemoryRepr(rows, columns) {}

SparseMatrix::SparseMatrix(const MemoryDump & dump)
    : MatrixMemoryRepr(dump.size(), dump.size() ? dump.at(0).size() : 0) {

    memory_dump_to_data(dump);
}

SparseMatrix::SparseMatrix(
    std::initializer_list<std::initializer_list<Rational>> init_list)
    : MatrixMemoryRepr(init_list.size(),
                       init_list.size() ? init_list.begin()->size() : 0) {

    std::size_t row = 0;
    for (const auto & list : init_list) {
        std::size_t col = 0;
        for (const auto & val : list) {
            if (val != 0) {
                _data.emplace(row, col, val);
            }
            ++col;
        }
        ++row;
    }
}

MatrixMemoryRepr * SparseMatrix::clone() const {
    return new SparseMatrix(*this);
}

Rational SparseMatrix::det() {
    if (_rows != _columns) {
        throw std::logic_error(
            "Determinant is undefined for non-square matrices.");
    }
    if (_det.has_value()) {
        return _det.value();
    }
    _det = calc_det();
    return _det.value();
}

Rational SparseMatrix::det() const {
    if (_rows != _columns) {
        throw std::logic_error(
            "Determinant is undefined for non-square matrices.");
    }
    if (_det.has_value()) {
        return _det.value();
    }
    return calc_det();
}

std::size_t SparseMatrix::rank() {
    if (_rank.has_value()) {
        return _rank.value();
    }
    _rank = calc_rank();
    return _rank.value();
}

std::size_t SparseMatrix::rank() const {
    if (_rank.has_value()) {
        return _rank.value();
    }
    return calc_rank();
}

void SparseMatrix::gem_swap_lines(
    SparseMatrix::MemoryDump & matrix,
    std::function<void(std::size_t, std::size_t, Rational)> && capture_fn) {

    std::size_t rows = matrix.size();
    std::size_t columns = rows ? matrix.at(0).size() : 0;
    for (std::size_t i = 0, column_index = 0;
         i < rows && column_index < columns; i++, column_index++) {
        if (matrix[i][column_index] == 0) {
            for (std::size_t j = i; j < rows; j++) {
                for (std::size_t h = column_index; h < columns; h++) {
                    if (matrix[i][h] == 0 && matrix[j][h] != 0) {
                        std::swap(matrix[i], matrix[j]);
                        capture_fn(i, j, matrix[i][j]);
                        break;
                    }
                }
            }
        }
    }
}

// https://www.math-cs.gordon.edu/courses/ma342/handouts/gauss.pdf
void SparseMatrix::gem_row_elim(
    SparseMatrix::MemoryDump & matrix,
    std::function<void(std::size_t, std::size_t, Rational)> && capture_fn) {

    std::size_t rows = matrix.size();
    std::size_t columns = rows ? matrix.at(0).size() : 0;
    for (std::size_t i = 0; i < rows - 1; i++) {
        for (std::size_t j = i + 1; j < rows; j++) {
            capture_fn(i, j, matrix[i][i]);
            Rational multiplier = matrix[j][i];
            for (std::size_t k = i; k < columns; k++) {
                matrix[j][k] =
                    (matrix[j][k] * matrix[i][i]) - (multiplier * matrix[i][k]);
            }
        }
    }
}

void SparseMatrix::gem() {
    auto matrix = dump();

    gem_swap_lines(matrix, [](std::size_t, std::size_t, Rational) { return; });
    gem_row_elim(matrix, [](std::size_t, std::size_t, Rational) { return; });

    memory_dump_to_data(matrix);
    _det.reset(); // invalidate determinant
}

// https://www.researchgate.net/publication/220337322_An_Efficient_and_Simple_Algorithm_for_Matrix_Inversion
void SparseMatrix::inverse() {
    if (_rows != _columns) {
        throw std::logic_error("Non-square matrices cannot be inverted.");
    }
    if (_det.has_value() && _det.value() == 0) {
        throw std::logic_error("Matrix is not invertible.");
    }

    std::queue<std::size_t> index_queue;
    std::set<std::size_t> visited_indexes;
    Rational determinant = 1;
    std::size_t dim = _rows;
    bool is_transposed = false;

    // check, if matrix should be transposed
    std::size_t missing_diagonal_el = 0;
    for (std::size_t i = 0; i < dim; i++) {
        if (!_data.count(MatrixElement(i, i, 0))) {
            missing_diagonal_el++;
            continue;
        }
        break;
    }

    if (missing_diagonal_el == dim) {
        transpose();
        is_transposed = true;
    }

    for (std::size_t i = 0; i < dim; i++) {
        index_queue.emplace(i);
    }

    auto matrix = dump();
    std::vector<std::pair<std::size_t, std::size_t>> row_swap_vec;
    gem_swap_lines(matrix, [&](std::size_t i, std::size_t j, Rational) {
        row_swap_vec.emplace_back(i, j);
    });

    for (std::size_t pivot_loc = index_queue.front(); !index_queue.empty();
         index_queue.pop(), pivot_loc = index_queue.front()) {
        Rational pivot = matrix[pivot_loc][pivot_loc];
        determinant *= pivot; // step 3
        if (pivot == 0) {
            if (!visited_indexes.count(pivot_loc)) {
                index_queue.emplace(pivot_loc);
                visited_indexes.emplace(pivot_loc);
                continue;
            }
            _det = 0;
            throw std::logic_error("Matrix is not invertible.");
        }
        // step 6
        for (std::size_t pivot_column = 0; pivot_column < dim; pivot_column++) {
            matrix[pivot_column][pivot_loc] /= Rational(-1) * pivot;
        }
        // step 7
        for (std::size_t j = 0; j < dim; j++) {
            if (pivot_loc != j) {
                for (std::size_t k = 0; k < dim; k++) {
                    if (pivot_loc != k) {
                        matrix[j][k] +=
                            matrix[pivot_loc][k] * matrix[j][pivot_loc];
                    }
                }
            }
        }
        // step 5
        for (std::size_t pivot_row = 0; pivot_row < dim; pivot_row++) {
            matrix[pivot_loc][pivot_row] /= pivot;
        }
        matrix[pivot_loc][pivot_loc] = Rational(1) / pivot; // step 8
    }

    for (const auto & [first_row, second_row] : row_swap_vec) {
        std::swap(matrix[first_row], matrix[second_row]);
    }

    _det = determinant;
    memory_dump_to_data(matrix);
    if (is_transposed) {
        transpose();
    }
}

void SparseMatrix::transpose() {
    std::set<MatrixElement> new_data;
    for (const auto & i : _data) {
        new_data.emplace(i.column, i.row, i.value);
    }
    std::swap(_rows, _columns);
    _data = std::move(new_data);
}

void SparseMatrix::unite(const MatrixMemoryRepr & other) {
    if (_rows != other.rows() || _columns != other.columns()) {
        throw std::logic_error("Matrices don't have the same dimensions.");
    }

    auto other_dump = other.dump();

    for (std::size_t i = 0; i < _rows; i++) {
        for (std::size_t j = 0; j < _columns; j++) {
            if (other_dump[i][j] != 0) {
                _data.emplace(i, j + _columns, other_dump[i][j]);
            }
        }
    }

    _columns += other.columns();
    _rank.reset(); // invalidate rank
}

std::vector<std::vector<Rational>> SparseMatrix::dump() const {
    std::vector<std::vector<Rational>> result;
    for (std::size_t i = 0; i < _rows; i++) {
        std::vector<Rational> row;
        for (std::size_t j = 0; j < _columns; j++) {
            row.emplace_back(0);
        }
        result.emplace_back(row);
    }
    for (const auto & e : _data) {
        result[e.row][e.column] = e.value;
    }

    return result;
}

void SparseMatrix::print(std::ostream & os) const {
    auto self_dump = dump();
    for (std::size_t i = 0; i < _rows; i++) {
        os << "[ ";
        for (std::size_t j = 0; j < _columns - 1; j++) {
            os << self_dump[i][j] << ", ";
        }
        os << self_dump[i][_columns - 1] << " ]";
        if (i != self_dump.size() - 1) {
            os << std::endl;
        }
    }
}

Rational SparseMatrix::calc_det() const {
    std::unique_ptr<MatrixMemoryRepr> matrix_copy(clone());
    auto matrix = matrix_copy->dump();

    std::vector<Rational> division_vec;
    gem_swap_lines(matrix, [&](std::size_t, std::size_t, Rational) {
        division_vec.emplace_back(-1);
    });

    gem_row_elim(matrix, [&](std::size_t, std::size_t, Rational val) {
        division_vec.emplace_back(val);
    });

    Rational det = 1;
    for (std::size_t i = 0; i < _rows; i++) {
        det *= matrix[i][i];
    }

    for (const auto & i : division_vec) {
        det /= i;
    }

    return det;
}

std::size_t SparseMatrix::calc_rank() const {
    std::size_t rank = 0;
    std::unique_ptr<MatrixMemoryRepr> matrix_copy(clone());
    matrix_copy->gem();
    auto copy_dump = matrix_copy->dump();

    for (std::size_t i = 0; i < _rows; i++) {
        for (std::size_t j = 0; j < _columns; j++) {
            if (copy_dump[i][j] == 0) {
                rank++;
                break;
            }
        }
    }

    return rank;
}
void SparseMatrix::memory_dump_to_data(
    const SparseMatrix::MemoryDump & matrix) {
    _data.clear();
    for (std::size_t i = 0; i < matrix.size(); i++) {
        for (std::size_t j = 0; j < matrix[i].size(); j++) {
            if (matrix[i][j] != 0) {
                _data.emplace(i, j, matrix[i][j]);
            }
        }
    }
}
