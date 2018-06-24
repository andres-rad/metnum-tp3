#include "./csvParser.h"

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
    fs.close();
}

void vector_to_csv(const vector<double>& vec, string file_path) {
    fstream fs;
    fs.open(file_path, fstream::out | fstream::trunc);
    for (uint i=0; i < vec.size(); i++) {
        fs << vec[i];
        if (i != vec.size() - 1) {
            fs << ",";
        }
    }
    fs.close();
}
