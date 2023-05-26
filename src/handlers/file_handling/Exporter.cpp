#include "Exporter.h"
#include "../../../libs/json.hpp"
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>

using json = nlohmann::json;

Exporter::Exporter(MatrixFactory factory) : FileHandler(factory) {}

static void write_sparse(json & file, const std::string & name, const Matrix & mx) {
    for (const auto & [pos, val] : mx) {
        std::string format = std::to_string(pos.row) + ":" + std::to_string(pos.column);
        file[name]["data"].emplace(format, val);
    }
}

static void write_dense(json & file, const std::string & name, const Matrix & mx) {
    std::vector<std::vector<double>> data_vec(mx.rows());
    for (auto & row : data_vec){
        row.resize(mx.columns());
    }
    for (const auto & [pos, val] : mx){
        data_vec[pos.row][pos.column] = val;
    }
    file[name]["data"].emplace("array", data_vec);
}

void Exporter::export_to_file(const std::unordered_map<std::string, Matrix> & vars,
                              const std::string & filename) {
    reset();
    std::ofstream outfile(filename);
    if (!outfile.is_open() || outfile.bad()){
        _status = "Couldn't open file: " + filename;
        _is_failed = true;
        return;
    }

    json out_data;
    for (const auto & [key, matrix] : vars){
        out_data[key].emplace("rows", matrix.rows());
        out_data[key].emplace("columns", matrix.columns());

        std::size_t number_of_non_zeroes = matrix.begin().distance(matrix.end());
        std::size_t max_non_zeroes = (1 - _factory.ratio()) * matrix.rows() * matrix.columns();

        out_data[key]["data"];
        if (number_of_non_zeroes < max_non_zeroes){
            write_sparse(out_data, key, matrix);
        } else {
            write_dense(out_data, key, matrix);
        }
    }
    outfile << std::setw(3) << out_data;
    if (!outfile.is_open() || outfile.fail() || outfile.bad()){
        _status = "An error occured while writing data.";
        _is_failed = true;
    } else {
        _status = "Write to " + filename + " finished successfully.";
        _is_failed = false;
    }
}