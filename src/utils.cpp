#include <set>
#include <iostream>
#include <utility>
#include <vector>
#include "utils.h"
#include "matrix.h"
#include "estructuras.h"

vector<double> resolverSistema(Matrix matrix, vector<double> vector);


vector<double> operator*(Matrix& matrix, vector<double> const &v) {
    vector<double> prod(matrix.n, 0);
    for (int i = 0; i < matrix.n; ++i) {
        double aux = 0;
        for (int j = 0; j < matrix.m; ++j) {
            aux += matrix[i][j]  *v[j];
        }
        prod[i] = aux;
    }
    return prod;
}

vector<double> cuadradosMinimos(Matrix& D,vector<double>& t){
    Matrix transposedD = D.transpose();
    Matrix A = transposedD * D;
    vector<double > b = transposedD * t;
    vector<double > result = resolverSistema(A, b);
    return result;
}

void row_operation(Matrix& matrix ,int indice_pivot, int indice_fila, vector<double> &b) {
    //realiza la resta de filas
    double row_elem = matrix[indice_fila][indice_pivot];
    if (row_elem != 0) {
        double m = row_elem / matrix[indice_pivot][indice_pivot];
        //Itero la fila del pivot y realizo la operacion con el elemento de la columna correspondiente de fila
        for (int i=indice_pivot;indice_pivot<matrix.n; i++) {
            double res = matrix[indice_fila][i] - matrix[indice_pivot][i] * m;
            matrix[indice_fila][i] = res ;
        }
        b[indice_fila] = b[indice_fila] - b[indice_pivot] * m; //opero sobre b tambien
    }
}

vector<double> resolverSistema(Matrix matrix, vector<double> b) {
    int n = matrix.n;
    //Descompongo usando Gauss
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            row_operation(matrix, i, j, b);
        }
    }
    vector<double> x(b);
    // Resuelvo el sistema
    x[n - 1] = x[n - 1] / matrix[n - 1][n - 1];
    for (int i = n - 2; i >= 0; --i) {
        for (int j = i + 1; j < n; ++j) {
            x[i] = x[i] - (matrix[i][j] * b[j]);
        }
        x[i] = x[i] / matrix[i][i];
    }

    return x;
}
