#ifndef SRC_CSVPARSER_HPP_
#define SRC_CSVPARSER_HPP_

#include <fstream>
#include <sstream>
#include <vector>

#include "./matrix.h"

#define debug(v) cerr << #v << ": " << v << endl

using namespace std;

Matrix csv_to_matrix(string file_path) {
    ifstream fs(file_path);
    string line;
    vector<vector<double> > mat;
    while(getline(fs, line)){
        stringstream lineStream(line);
        string item;
        vector<double> fila;
        while(getline(lineStream, item, ',')){
            fila.push_back(stod(item));
        }
        mat.push_back(fila);
    }

    return Matrix(mat);
}

void matrix_to_csv(Matrix& matrix, string file_path) {
    fstream fs;
    fs.open(file_path, fstream::out | fstream::trunc);
    fs << matrix;
}

#endif  // SRC_CSVPARSER_HPP_
