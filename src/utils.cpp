#include <set>
#include <iostream>
#include <utility>
#include <vector>
#include "utils.h"
#include "matrix.h"
#include "estructuras.h"

vector<double> resolverSistema(Matrix matrix, vector<double> vector);

set<Coord> discretizarRayo(Coord inicio, Coord fin){
    //Sacado de http://eugen.dedu.free.fr/projects/bresenham/
    set<Coord> res;

    int deltaX = fin.x - inicio.x;
    int deltaY = fin.y - inicio.y;
    int dobleDX = 2 * deltaX;
    int dobleDY = 2 * deltaY;
    int curY = inicio.y;
    int curX = inicio.x;
    int stepX = 1;
    int stepY = 1;
    int ultError, error;

    if(deltaX < 0) {
        stepX = -1;
        deltaX = -deltaX;
    }
    if(deltaY < 0) {
        stepY = -1;
        deltaY = -deltaY;
    }

    if(dobleDX >= dobleDY){ //Pendiente menor a identidad
        ultError = error = deltaX;

        for(int i = 0; i < deltaX; i++){
            curX += stepX;
            error += dobleDY;

            if(error > dobleDX){
                curY += stepY;
                error -= dobleDX;
            }

            if(error + ultError < dobleDX){
                res.insert(Coord(curX, curY-stepY));
            } else if (error + ultError > 2 * deltaX){
                res.insert(Coord(curX - stepX, curY));
            }
            res.insert(Coord(curX, curY));
            ultError = error;
        }
    } else { //Pendiente mayor a identidad
        ultError = error = deltaY;
        for(int i = 0; i < deltaY; i++){
            curY += stepY;
            error += 2 * deltaX;

            if(error > 2 * deltaY){
                    curX += stepX;
                    error -= 2 * deltaY;
                    if(error + ultError < 2 * deltaY){
                        res.insert(Coord(curX - stepX, curY));
                    } else if (error + ultError > 2 * deltaY){
                        res.insert(Coord(curX, curY - stepY));
                    }
            }
            res.insert(Coord(curX, curY));
            ultError = error;
        }
    }
    return res;
}

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
