#include "Matrix.h"
#include "../representations/MatrixMemoryRepr.h"
#include "MatrixFactory.h"
#include <queue>
#include <set>
#include <stdexcept>

constexpr auto default_capture = [](std::size_t, std::size_t) { return; };

void Matrix::gem_swap_rows(
    std::function<void(std::size_t, std::size_t)> && capture_fn) {

    for (std::size_t i = 0, column_index = 0;
         i < rows() && column_index < columns(); i++, column_index++) {
        if (_matrix->at(i, column_index).value() == 0) {
            for (std::size_t j = i; j < rows(); j++) {
                for (std::size_t h = column_index; h < columns(); h++) {
                    if (_matrix->at(i, h).value() == 0 &&
                        _matrix->at(j, h).value() != 0) {
                        _matrix->swap_rows(i, j);
                        capture_fn(i, j);
                        break;
                    }
                }
            }
        }
    }
}

// https://www.math-cs.gordon.edu/courses/ma342/handouts/gauss.pdf
void Matrix::gem_row_elim(
    std::function<void(std::size_t, std::size_t)> && capture_fn) {

    for (std::size_t i = 0; i < columns(); i++) {
        for (std::size_t j = i + 1; j < rows(); j++) {
            capture_fn(i, j);
            double multiplier = _matrix->at(j, i).value();
            for (std::size_t k = i; k < columns(); k++) {
                _matrix->modify(
                    j, k,
                    (_matrix->at(j, k).value() * _matrix->at(i, i).value() -
                     (multiplier * _matrix->at(i, k).value())));
            }
        }
    }
}

void Matrix::optimize() {
    auto new_ptr = _factory.convert(_matrix.get());
    if (new_ptr == _matrix.get()) {
        return;
    }
    _matrix.reset(new_ptr);
}

Matrix::Matrix(std::size_t rows, std::size_t columns, MatrixFactory factory)
    : _factory(factory) {
    _matrix = std::unique_ptr<MatrixMemoryRepr>(
        _factory.get_initial_repr(rows, columns));
}

Matrix::Matrix(std::initializer_list<std::initializer_list<double>> init,
               MatrixFactory factory)
    : _factory(factory) {
    _matrix =
        std::unique_ptr<MatrixMemoryRepr>(_factory.get_initial_repr(init));
}

Matrix::Matrix(const MatrixMemoryRepr & repr, MatrixFactory factory)
    : _matrix(repr.clone()), _factory(factory) {
    optimize();
}

Matrix::Matrix(IteratorWrapper begin, IteratorWrapper end,
               MatrixFactory factory)
    : _matrix(factory.get_initial_repr(std::move(begin), std::move(end))),
      _factory(factory) {}

Matrix::Matrix(double val) : _factory(0.5) {
    _matrix = std::unique_ptr<MatrixMemoryRepr>(_factory.get_initial_repr(val));
}

Matrix::Matrix(const Matrix & src)
    : _matrix(src._matrix->clone()), _factory(src._factory) {}

Matrix::Matrix(Matrix && src) noexcept
    : _matrix(std::move(src._matrix)), _factory(src._factory) {}

Matrix & Matrix::operator=(const Matrix & src) {
    if (this != &src) {
        _matrix = std::unique_ptr<MatrixMemoryRepr>(src._matrix->clone());
        _factory = src._factory;
    }
    return *this;
}

Matrix & Matrix::operator=(Matrix && src) noexcept {
    if (this != &src) {
        _matrix = std::move(src._matrix);
        _factory = src._factory;
    }
    return *this;
}

Matrix Matrix::operator+(const Matrix & other) const {
    if (rows() != other.rows() || columns() != other.columns()) {
        throw std::invalid_argument(
            "Matrix addition: dimensions are not matching.");
    }
    Matrix result(*this);
    for (const auto & [pos, val] : other) {
        result._matrix->add(pos.row, pos.column, val);
    }
    result.optimize();
    return result;
}

Matrix Matrix::operator-(const Matrix & other) const {
    if (rows() != other.rows() || columns() != other.columns()) {
        throw std::invalid_argument(
            "Matrix subtraction: dimensions are not matching.");
    }
    Matrix result(*this);
    for (const auto & [pos, val] : other) {
        result._matrix->add(pos.row, pos.column, val * -1);
    }
    result.optimize();
    return result;
}

Matrix Matrix::operator*(const Matrix & other) const {
    if (rows() == 1 && columns() == 1) {
        return _matrix->at(0, 0).value() * other;
    }

    if (other.rows() == 1 && other.columns() == 1) {
        return other._matrix->at(0, 0).value() * *this;
    }

    if (columns() != other.rows()) {
        throw std::invalid_argument(
            "Matrix multiplication: invalid matrix dimensions.");
    }
    Matrix result(rows(), other.columns(), _factory);

    for (std::size_t i = 0; i < result.rows(); i++) {
        for (std::size_t j = 0; j < result.columns(); j++) {
            double result_element = 0;
            for (std::size_t k = 0; k < columns(); k++) {
                result_element +=
                    _matrix->at(i, k).value() * other._matrix->at(k, j).value();
            }
            result._matrix->modify(i, j, result_element);
        }
    }
    result.optimize();
    return result;
}

Matrix operator*(double scalar, const Matrix & mx) {
    Matrix result(mx);
    for (const auto & [pos, val] : mx) {
        double new_value = val * scalar;
        result._matrix->modify(pos.row, pos.column, new_value);
    }
    result.optimize();
    return result;
}

std::size_t Matrix::rows() const { return _matrix->rows(); }

std::size_t Matrix::columns() const { return _matrix->columns(); }

IteratorWrapper Matrix::begin() const { return _matrix->begin(); }

IteratorWrapper Matrix::end() const { return _matrix->end(); }

Matrix Matrix::transpose() const {
    Matrix transposed(columns(), rows(), _factory);
    for (const auto & [pos, val] : *this) {
        transposed._matrix->modify(pos.column, pos.row, val);
    }
    return transposed;
}

Matrix Matrix::unite(const Matrix & first, const Matrix & second) {
    if (first.columns() != second.columns()) {
        throw std::invalid_argument("Unite: matrix dimension mismatch");
    }
    Matrix united(first.rows() + second.rows(), first.columns(),
                  first._factory);
    for (const auto & [pos, val] : first) {
        united._matrix->modify(pos.row, pos.column, val);
    }
    for (const auto & [pos, val] : second) {
        united._matrix->modify(pos.row + first.rows(), pos.column, val);
    }
    united.optimize();
    return united;
}

Matrix Matrix::cut(std::size_t new_size_rows, std::size_t new_size_columns,
                   std::size_t offset_rows, std::size_t offset_columns) const {
    if (!new_size_rows || !new_size_columns) {
        throw std::invalid_argument("Cut: invalid new dimensions.");
    }
    if (new_size_rows + offset_rows > rows() ||
        new_size_columns + offset_columns > columns()) {
        throw std::invalid_argument("Cut: invalid new dimensions or offset.");
    }

    Matrix result(new_size_rows, new_size_columns, _factory);
    for (std::size_t i = 0; i < new_size_rows; i++) {
        for (std::size_t j = 0; j < new_size_columns; j++) {
            result._matrix->modify(
                i, j, _matrix->at(offset_rows + i, offset_columns + j).value());
        }
    }
    result.optimize();
    return result;
}

static inline bool matrix_is_a_number(const Matrix & mx) {
    return mx.rows() == 1 && mx.columns() == 1;
}

Matrix Matrix::cut(const Matrix & new_size_rows_mx,
                   const Matrix & new_size_columns_mx,
                   const Matrix & offset_rows_mx,
                   const Matrix & offset_columns_mx) const {
    if (!matrix_is_a_number(new_size_rows_mx) ||
        !matrix_is_a_number(new_size_columns_mx) ||
        !matrix_is_a_number(offset_rows_mx) ||
        !matrix_is_a_number(offset_columns_mx)) {
        throw std::invalid_argument("Cut: Invalid arguments for conversion.");
    }

    std::size_t new_size_rows = new_size_rows_mx._matrix->at(0, 0).value();
    std::size_t new_size_column = new_size_columns_mx._matrix->at(0, 0).value();
    std::size_t offset_rows = offset_rows_mx._matrix->at(0, 0).value();
    std::size_t offset_columns = offset_columns_mx._matrix->at(0, 0).value();

    return cut(new_size_rows, new_size_column, offset_rows, offset_columns);
}

// https://www.researchgate.net/publication/220337322_An_Efficient_and_Simple_Algorithm_for_Matrix_Inversion
// implementing this algorithm shortened my expected lifespan by like 5 years
// the algorithm is described in mathematical notation and in C, however,
// the order of steps between these descriptions doesn't match up, the
// mathematical notation is simply wrong. if you ever decide to reimplement this
// (please don't), then don't bother with the math notation and rewrite the
// C code provided in the link (because it's wrong as well lmao, it doesn't
// make any sense)
Matrix Matrix::inverse() const {
    if (rows() != columns()) {
        throw std::logic_error("Non-square matrices cannot be inverted.");
    }

    Matrix result(*this);

    std::queue<std::size_t> index_queue;
    std::set<std::size_t> visited_indexes;
    std::size_t dim = rows();

    for (std::size_t i = 0; i < dim; i++) {
        index_queue.emplace(i);
    }

    std::vector<std::pair<std::size_t, std::size_t>> row_swap_vec;
    result.gem_swap_rows(
        [&](std::size_t i, std::size_t j) { row_swap_vec.emplace_back(i, j); });

    for (std::size_t pivot_loc = index_queue.front(); !index_queue.empty();
         index_queue.pop(), pivot_loc = index_queue.front()) {
        double pivot = result._matrix->at(pivot_loc, pivot_loc).value();
        if (pivot == 0) {
            if (!visited_indexes.count(pivot_loc)) {
                index_queue.emplace(pivot_loc);
                visited_indexes.emplace(pivot_loc);
                continue;
            }
            throw std::runtime_error("Matrix is not invertible.");
        }
        // step 6
        for (std::size_t pivot_column = 0; pivot_column < dim; pivot_column++) {
            result._matrix->modify(
                pivot_column, pivot_loc,
                result._matrix->at(pivot_column, pivot_loc).value() / (-pivot));
        }
        // step 7
        for (std::size_t j = 0; j < dim; j++) {
            if (pivot_loc != j) {
                for (std::size_t k = 0; k < dim; k++) {
                    if (pivot_loc != k) {
                        result._matrix->add(
                            j, k,
                            result._matrix->at(pivot_loc, k).value() *
                                result._matrix->at(j, pivot_loc).value());
                    }
                }
            }
        }
        // step 5 / 8
        for (std::size_t pivot_row = 0; pivot_row < dim; pivot_row++) {
            result._matrix->modify(
                pivot_loc, pivot_row,
                result._matrix->at(pivot_loc, pivot_row).value() / pivot);
        }
        result._matrix->modify(pivot_loc, pivot_loc,
                               1 / pivot); // step 8 / 9
    }

    for (const auto & [first_row, second_row] : row_swap_vec) {
        result._matrix->swap_rows(first_row, second_row);
    }

    result.optimize();
    return result;
}

std::optional<double> Matrix::det() const {
    if (rows() != columns()) {
        return std::nullopt;
    }
    Matrix copied_matrix(*this);

    std::vector<double> division_vec;
    copied_matrix.gem_swap_rows(
        [&](std::size_t, std::size_t) { division_vec.emplace_back(-1); });

    copied_matrix.gem_row_elim([&](std::size_t i, std::size_t) {
        division_vec.emplace_back(copied_matrix._matrix->at(i, i).value());
    });

    double det = 1;
    for (std::size_t i = 0; i < rows(); i++) {
        det *= copied_matrix._matrix->at(i, i).value();
    }

    for (const auto & i : division_vec) {
        if (i == 0) {
            return 0;
        }
        det /= i;
    }

    return det;
}

std::size_t Matrix::rank() const {
    Matrix copied_matrix(*this);
    copied_matrix.gem();

    std::size_t rank = 0;
    for (std::size_t i = 0; i < rows(); i++) {
        for (std::size_t j = 0; j < columns(); j++) {
            if (copied_matrix._matrix->at(i, j).value() != 0) {
                rank++;
                break;
            }
        }
    }

    return rank;
}

std::ostream & operator<<(std::ostream & os, const Matrix & mx) {
    os << *(mx._matrix);
    return os;
}

Matrix Matrix::gem() const {
    Matrix result(*this);
    result.gem_swap_rows();
    result.gem_row_elim();
    result.optimize();
    return result;
}