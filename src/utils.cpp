#include <algorithm>
#include <iostream>
#include <random>
#include <set>
#include <utility>
#include <vector>

#include "utils.h"
#include "matrix.h"
#include "estructuras.h"

#define debug(v) cerr << #v << ": " << v << endl
#define debugVec(v) for(auto i:v) cerr << i << ", "; cerr<<endl
#define mu 1e-8
#define MAXITER 500

vector<double> resolverSistema(Matrix matrix, vector<double> vector);

double euclidianNorm(vector<double> &x) {
    double sum = 0;
    for (unsigned int i = 0; i < x.size(); i++) {
        sum += x[i] * x[i];
    }
    return sqrt(sum);
}

void normalize(vector<double> &x) {
    double norm = euclidianNorm(x);
    for (unsigned int i = 0; i < x.size(); i++) {
        x[i] = x[i] / norm;
    }
}

vector<double> randomVector(int i) {
    vector<double> vector(i, 0);
    random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<double> dist(-10.0, 10.0);
    for (int j = 0; j < i; ++j) {
        vector[j] = dist(mt) / 10;
    }
    normalize(vector);
    return vector;
}

vector<double> gaussSeidel(Matrix matrix, std::vector<double> b) {
    vector<double> x = randomVector(matrix.n);

    for (int iter = 0; iter < MAXITER; iter++) {
        for (int j = 0; j < matrix.n; j++) {
            double sum = 0;
            for (int i = 0; i < matrix.n; i++) {
                if (i != i) { sum += matrix[j][i] * x[i]; }
            }
            x[j] = (b[j] - sum) / matrix[j][j];
        }
        // debugVec(x);
    }
    return x;
}

vector<double> operator*(Matrix &matrix, vector<double> const &v) {
    vector<double> prod(matrix.n, 0);
    for (int i = 0; i < matrix.n; ++i) {
        double aux = 0;
        for (int j = 0; j < matrix.m; ++j) {
            aux += matrix[i][j] * v[j];
        }
        prod[i] = aux;
    }
    return prod;
}

vector<double> cuadradosMinimos(Matrix &D, vector<double> &t) {
    Matrix transposedD = D.transpose();
    Matrix A = transposedD * D;

    vector<double> b = transposedD * t;
    vector<double> result = resolverSistema(A, b);

    return result;
}

void row_operation(Matrix &matrix, int indice_pivot, int indice_fila, vector<double> &b) {
    // realiza la resta de filas
    double row_elem = matrix[indice_fila][indice_pivot];
    if (abs(row_elem) != 0) {
        double m = row_elem / matrix[indice_pivot][indice_pivot];
        // Itero la fila del pivot y realizo la operacion con el elemento de la columna correspondiente de fila
        for (int i = indice_pivot; i < matrix.n; i++) {
            double res = matrix[indice_fila][i] - matrix[indice_pivot][i] * m;
            matrix[indice_fila][i] = res < mu ? 0: res;
        }
        double temp = b[indice_fila] - b[indice_pivot] * m;
        b[indice_fila] = temp < mu ? 0: temp;  // opero sobre b tambien
    }
}

int findPivot(Matrix &matrix, int k) {
    int pivot = -1;
    double pivot_value = 0;
    for (int i = k; i < matrix.n; i++) {
        if (abs(matrix[i][k]) > abs(pivot_value)) {
            pivot = i;
            pivot_value = matrix[i][k];
        }
    }
    return pivot;
}

vector<double> resolverSistema(Matrix matrix, vector<double> b) {
    int n = matrix.n;
    std::vector<int> permutations;
    for (int i = 0; i < n; i++) { permutations.push_back(i); }
    // Descompongo usando Gauss
    for (int i = 0; i < n; ++i) {

        int indice_pivot = findPivot(matrix, i);
        swap(matrix[i], matrix[indice_pivot]);
        swap(b[i], b[indice_pivot]);
        swap(permutations[indice_pivot], permutations[i]);

        for (int j = i + 1; j < n; ++j) {
            row_operation(matrix, i, j, b);
        }
    }

    // Resuelvo el sistema

    b[n - 1] /= matrix[n - 1][n - 1];
    for (int i = n - 2; i >= 0; i--) {
        for (int j = n - 1; j > i; j--) {
            b[i] = b[i] - (matrix[i][j] * b[j]);
        }
            b[i] = b[i] / matrix[i][i];
    }

    std::vector<double> result(n, 0);
    for (int i = 0; i < n; ++i) {
        result[permutations[i]] = b[i];
    }
    debugVec(result);

    return result;
}

void agregarRuido(vector<double> &t, int tipo, double porcentaje, double max, double std) {
    int cantidad = (int) t.size() * porcentaje / 100.0;
    int positions[t.size()];
    for (unsigned int i = 0; i < t.size(); i++) {
        positions[i] = i;
    }
    random_shuffle(&positions[0], &positions[t.size()]);
    switch (tipo) {
        case 1:
            guaussianNoise(t, cantidad, positions, max, std);
            break;
        case 2:
            poissonNoise(t, cantidad, positions, max, std);
            break;
        case 3:
            randomNoise(t, cantidad, positions, max);
            break;
        case 4:
            outlierNoise(t, cantidad, positions, max);
            break;
    }
}

void guaussianNoise(vector<double> &t, int cantidad, int positions[], double max, double std) {
    default_random_engine generator;
    normal_distribution<float> dist(0.0, std);
    double ruido;
    for (int i = 0; i < cantidad; i++) {
        do {
            ruido = dist(generator);
        } while (ruido > max);
        t[positions[i]] = t[positions[i]] + ruido;
        t[positions[i]] = (0 < t[positions[i]]) ? t[positions[i]] : 0;
    }
}

void poissonNoise(vector<double> &t, int cantidad, int positions[], double max, double std) {
    default_random_engine generator;
    poisson_distribution<int> dist(1);
    double ruido;
    for (int i = 0; i < cantidad; i++) {
        do {
            ruido = dist(generator) * std;
        } while (ruido > max);
        t[positions[i]] = t[positions[i]] + ruido;
        t[positions[i]] = (0 < t[positions[i]]) ? t[positions[i]] : 0;
    }
}

void randomNoise(vector<double> &t, int cantidad, int positions[], double max) {
    for (int i = 0; i < cantidad; i++) {
        int ruido = (rand() % static_cast<int>(max * 2 + 1)) - max;
        t[positions[i]] = t[positions[i]] + ruido;
        t[positions[i]] = (0 < t[positions[i]]) ? t[positions[i]] : 0;
    }
}

void outlierNoise(vector<double> &t, int cantidad, int positions[], double max) {
    for (int i = 0; i < cantidad; i++) {
        int ruido = (rand() % 2) * max;
        t[positions[i]] = ruido;
    }
}
