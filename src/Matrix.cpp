#include "Matrix.h"
#include <queue>
#include <set>

constexpr auto default_capture = [](std::size_t, std::size_t) { return; };

void Matrix::gem_swap_rows(std::function<void(std::size_t, std::size_t)> &&
                               capture_fn = default_capture) {

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
void Matrix::gem_row_elim(std::function<void(std::size_t, std::size_t)> &&
                              capture_fn = default_capture) {

    for (std::size_t i = 0; i < rows() - 1; i++) {
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
    if (new_ptr == _matrix.get()){
        return;
    }
    _matrix.reset(new_ptr);
}

Matrix::Matrix(std::size_t rows, std::size_t columns) {
    _matrix = std::unique_ptr<MatrixMemoryRepr>(
        _factory.get_initial_repr(rows, columns));
}

Matrix::Matrix(std::initializer_list<std::initializer_list<double>> init) {
    _matrix =
        std::unique_ptr<MatrixMemoryRepr>(_factory.get_initial_repr(init));
}

Matrix::Matrix(const Matrix & src)
    : _matrix(src._matrix->clone()), _det(src._det), _rank(src._rank) {}

Matrix::Matrix(Matrix && src) noexcept
    : _matrix(std::move(src._matrix)), _det(src._det), _rank(src._rank) {}

Matrix & Matrix::operator=(const Matrix & src) {
    if (this != &src) {
        _matrix = std::unique_ptr<MatrixMemoryRepr>(src._matrix->clone());
        _det = src._det;
        _rank = src._rank;
    }
    return *this;
}

Matrix & Matrix::operator=(Matrix && src) noexcept {
    if (this != &src) {
        _matrix = std::move(src._matrix);
        _det = src._det;
        _rank = src._rank;
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
    if (columns() != other.rows()) {
        throw std::invalid_argument(
            "Matrix multiplication: invalid matrix dimensions.");
    }
    Matrix result(rows(), other.columns());

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

void Matrix::transpose() {
    Matrix transposed(columns(), rows());
    for (const auto & [pos, val] : *this) {
        transposed._matrix->modify(pos.column, pos.row, val);
    }
    _matrix = std::move(transposed._matrix);
}

void Matrix::unite(const Matrix & other) {
    if (columns() != other.columns()) {
        throw std::invalid_argument("Unite: matrix dimension mismatch");
    }
    Matrix united(rows() + other.rows(), columns());
    for (const auto & [pos, val] : *this) {
        united._matrix->modify(pos.row, pos.column, val);
    }
    for (const auto & [pos, val] : other) {
        united._matrix->modify(pos.row + rows(), pos.column, val);
    }
    united.optimize();
    _matrix = std::move(united._matrix);
}

void Matrix::cut(std::size_t) {}

void Matrix::inverse() {
    if (rows() != columns()) {
        throw std::logic_error("Non-square matrices cannot be inverted.");
    }
    if (_det.has_value() && _det.value() == 0) {
        throw std::logic_error("Matrix is not invertible.");
    }

    std::queue<std::size_t> index_queue;
    std::set<std::size_t> visited_indexes;
    double determinant = 1;
    std::size_t dim = rows();
    bool is_transposed = false;

    // check, if matrix should be transposed
    std::size_t missing_diagonal_el = 0;
    for (std::size_t i = 0; i < dim; i++) {
        if (_matrix->at(i, i).value() == 0) {
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

    std::vector<std::pair<std::size_t, std::size_t>> row_swap_vec;
    gem_swap_rows(
        [&](std::size_t i, std::size_t j) { row_swap_vec.emplace_back(i, j); });

    for (std::size_t pivot_loc = index_queue.front(); !index_queue.empty();
         index_queue.pop(), pivot_loc = index_queue.front()) {
        double pivot = _matrix->at(pivot_loc, pivot_loc).value();
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
            _matrix->modify(pivot_column, pivot_loc,
                            _matrix->at(pivot_column, pivot_loc).value() /
                                (-pivot));
        }
        // step 7
        for (std::size_t j = 0; j < dim; j++) {
            if (pivot_loc != j) {
                for (std::size_t k = 0; k < dim; k++) {
                    if (pivot_loc != k) {
                        _matrix->add(j, k,
                                     _matrix->at(pivot_loc, k).value() *
                                         _matrix->at(j, pivot_loc).value());
                    }
                }
            }
        }
        // step 5 / 8
        for (std::size_t pivot_row = 0; pivot_row < dim; pivot_row++) {
            _matrix->modify(pivot_loc, pivot_row,
                            _matrix->at(pivot_loc, pivot_row).value() / pivot);
        }
        _matrix->modify(pivot_loc, pivot_loc,
                        1 / pivot); // step 8 / 9
    }

    for (const auto & [first_row, second_row] : row_swap_vec) {
        _matrix->swap_rows(first_row, second_row);
    }

    _det = determinant;
    if (is_transposed) {
        transpose();
    }
    optimize();
}

std::optional<double> Matrix::det() {
    if (rows() != columns()) {
        return std::nullopt;
    }
    if (_det.has_value()) {
        return _det;
    }
    _det = calc_det();
    return _det;
}

std::optional<double> Matrix::det() const {
    if (rows() != columns()) {
        return std::nullopt;
    }
    if (_det.has_value()) {
        return _det;
    }
    return calc_det();
}

std::size_t Matrix::rank() {
    if (_rank.has_value()) {
        return _rank.value();
    }
    _rank = calc_rank();
    return _rank.value();
}

std::size_t Matrix::rank() const {
    if (_rank.has_value()) {
        return _rank.value();
    }
    return calc_rank();
}

std::ostream & operator<<(std::ostream & os, const Matrix & mx) {
    os << *(mx._matrix);
    return os;
}

void Matrix::gem() {
    gem_swap_rows();
    gem_row_elim();
    optimize();
}

std::optional<double> Matrix::calc_det() const {
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
        if (i == 0){
            return 0;
        }
        det /= i;
    }

    return det;
}

std::size_t Matrix::calc_rank() const {
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
