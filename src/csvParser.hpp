#ifndef _CSVPARSER_H_
#define _CSVPARSER_H_

#include <fstream>
#include <vector>

#include "matrix.h"

using namespace std;

Matrix csv_to_matrix(string file_path){
    int size;
    int idx = 0;
    fstream fs;

    fs.open(file_path, fstream::in);
    fs >> size;

    Matrix m = Matrix(size, size);
    string item;

    while(getline(fs, item, ',')){
        m[idx/size][idx%size] = stoi(item);
        idx++;
    }

    return m;
}

void matrix_to_csv(Matrix& matrix, string file_path){
    fstream fs;
    fs.open(file_path, fstream::out | fstream::trunc);
    fs << matrix;
}

#endif
