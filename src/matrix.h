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

    Matrix(int n);

    Matrix(vector<vector<double> > &mat);

    double getElem(int i, int j) const;

    void setElem(int i, int j, double val);

    Matrix transpose();

    Matrix prodTranspuesto(Matrix& at);

    pair<int, int> dimensions() const;

    //De eliminacion gausiana
    void swap_rows(int r1, int r2);

    int findPivot(int row_init, int col);

    void row_operation(int indice_pivot, int fila, vector<double> &b);

    // vector<int> g_elimination(vector<double> &b);// Retorna el vector de permutaciones
    void g_elimination(vector<double> &b);

    friend Matrix operator*(const Matrix &m1, const Matrix &m2);

    friend Matrix operator*(double e, const Matrix &m);


    int n, m;

private:
    unordered_map<int, unordered_map<int, double> > matrix;
};

Matrix operator*(const Matrix &m1, const Matrix &m2);

Matrix operator+(const Matrix &a, const Matrix &b);

Matrix operator-(const Matrix &a, const Matrix &b);

Matrix operator*(double e, const Matrix &m);

ostream &operator<<(ostream &os, const Matrix &matrix);

vector<double> operator*(Matrix &matrix, vector<double> &x);

Matrix identity(int n);

Matrix vec_to_matrix(vector<double> elems, int n, int m);


#endif  // SRC_MATRIX_H_
