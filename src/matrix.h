#ifndef SRC_MATRIX_H_
#define SRC_MATRIX_H_

#include <utility>
#include <vector>
#include <unordered_map>
#include <iostream>

using namespace std;

class Matrix {

 public:
    Matrix(int n, int m);

    Matrix(int n, int m, double init_val);

    Matrix(vector<vector<double> > &mat);

    Matrix();

    void push_row(vector<double> row);

    pair<int, int> dimensions() const;

    Matrix transpose();

    vector<double>& operator[] (int i);

    int n, m;

 private:
     vector<vector<double> > matrix;
};


Matrix operator*(Matrix &m1, Matrix &m2);

Matrix operator+(Matrix &a, Matrix &b);

Matrix operator-(Matrix &a, Matrix &b);

Matrix operator*(double p, Matrix &m);

ostream &operator<<(ostream &os, Matrix &matrix);

Matrix identity(int n);

Matrix vec_to_matrix(vector<double> elems, int n, int m);

#endif  // SRC_MATRIX_H_
