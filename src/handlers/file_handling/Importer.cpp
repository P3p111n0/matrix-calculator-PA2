#include "Importer.h"
#include "../../../libs/json.hpp"
#include "../../iterators/DenseMatrixIterator.h"
#include "../../iterators/SparseMatrixIterator.h"
#include <fstream>
#include <stdexcept>
#include <vector>

using json = nlohmann::json;

Importer::Importer(MatrixFactory factory) : FileHandler(factory) {}

static bool check_contents(const json & json_file,
                           const std::string & mx_name) {
    if (!json_file[mx_name].contains("rows") ||
        !json_file[mx_name].contains("columns") ||
        !json_file[mx_name].contains("data")) {
        return false;
    }
    if (!json_file[mx_name]["rows"].is_number() ||
        !json_file[mx_name]["columns"].is_number()) {
        return false;
    }
    if (json_file[mx_name]["data"].contains("array") &&
        !json_file[mx_name]["data"]["array"].is_array()) {
        return false;
    } else if (!json_file[mx_name]["data"].contains("array") &&
               !json_file[mx_name]["data"].is_null()) {
        for (const auto & [key, val] : json_file[mx_name]["data"].items()) {
            if (!json_file[mx_name]["data"][key].is_number()) {
                return false;
            }
        }
    }
    return true;
}

Matrix read_dense(json & json_data, const std::string & name,
                  const MatrixDimensions & dims,
                  const MatrixFactory & factory) {
    std::vector<std::vector<double>> mx_data(dims.rows());
    for (auto & row : mx_data) {
        row.resize(dims.columns());
    }
    mx_data = json_data[name]["data"]["array"]
                  .get<std::vector<std::vector<double>>>();
    if (mx_data.size() != dims.rows()){
        throw std::runtime_error("Invalid array dimensions in matrix.");
    }
    for (const auto & row : mx_data){
        if (row.size() != dims.columns()){
            throw std::runtime_error("Invalid array dimensions in matrix,");
        }
    }

    return {{new DenseMatrixIterator(&dims, mx_data, 0, 0)},
            {new DenseMatrixIterator(&dims, mx_data, dims.rows(), 0)},
            factory};
}

Matrix read_sparse(json & json_data, const std::string & name,
                   const MatrixDimensions & dims,
                   const MatrixFactory & factory) {
    std::map<Position, double> mx_data;
    for (const auto & [key, val] : json_data[name]["data"].items()) {
        std::stringstream oss(key);
        std::size_t row, col;
        char c;

        double value_at_key = json_data[name]["data"][key].get<double>();
        if (value_at_key == 0){
            continue;
        }
        if (!(oss >> row) || !(oss >> c) || c != ':' || !(oss >> col)) {
            throw std::runtime_error("Unknown key: " + key);
        }
        if (row >= dims.rows() || col >= dims.columns()) {
            throw std::runtime_error("Unknown position: " + key);
        }
        mx_data[{row, col}] = value_at_key;
    }
    return {{new SparseMatrixIterator(&dims, mx_data.begin(), mx_data.end())},
            {new SparseMatrixIterator(&dims, mx_data.end(), mx_data.end())},
            factory};
}

void Importer::import_from_file(std::unordered_map<std::string, Matrix> & vars,
                                const std::string & filename) {
    reset();
    std::unordered_map<std::string, Matrix> loaded_matrices;
    std::ifstream infile(filename);
    if (!infile.is_open() || infile.fail() || infile.bad()) {
        _status = "Couldn't open file: " + filename;
        _is_failed = true;
        return;
    }

    json input_data;
    try {
        input_data = json::parse(infile);
    } catch (std::exception & e) {
        _status = "An error occurred during json parsing.";
        _is_failed = true;
        return;
    }

    for (const auto & [key, data] : input_data.items()) {
        if (!check_contents(input_data, key)) {
            _status = "Error while reading data of matrix: " + key;
            _is_failed = true;
            return;
        }

        std::size_t rows = input_data[key]["rows"].get<std::size_t>();
        std::size_t columns = input_data[key]["columns"].get<std::size_t>();
        if (!rows || !columns) {
            _status = "Invalid dimensions of matrix: " + key;
            _is_failed = true;
            return;
        }
        try {
            if (input_data[key]["data"].is_null()) {
                loaded_matrices.emplace(key, Matrix(rows, columns, _factory));
                continue;
            } else if (input_data[key]["data"].contains("array")) {
                loaded_matrices.emplace(
                    key,
                    read_dense(input_data, key, {rows, columns}, _factory));
            } else {
                loaded_matrices.emplace(
                    key,
                    read_sparse(input_data, key, {rows, columns}, _factory));
            }
        } catch (std::exception & e) {
            _status = "An error occurred while reading matrix: " + key + '\n';
            _status += e.what();
            _is_failed = true;
            return;
        }
    }
    for (auto & [key, val] : loaded_matrices){
        if (vars.count(key)){
            _status += "Overwriting variable: " + key + '\n';
        }
        vars.erase(key);
        vars.emplace(key, val);
    }
    _status += "Import from file " + filename + " successfully finished";
}
