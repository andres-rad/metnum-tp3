#include <algorithm>
#include <iostream>
#include <random>
#include <set>
#include <utility>
#include <vector>

#include "utils.h"
#include "matrix.h"
#include "estructuras.h"
#include "./defines.h"

#define mu 1e-8
#define MAXITER 100
bool iterative = false;

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
                if (i != j) { sum += matrix.getElem(j,i) * x[i]; }
            }
            x[j] = (b[j] - sum) / matrix.getElem(j, j);
        }
//         debugVec(x);
    }
    return x;
}
vector<double> cuadradosMinimos(Matrix &D, vector<double> &t) {
    Matrix transposedD = D.transpose();
    Matrix A = transposedD.prodTranspuesto(D);
    vector<double> b = transposedD * t;
    vector<double> result;
    if(iterative) {
        result = gaussSeidel(A, b);
    }else{
        result = resolverSistema(A, b);
    }
    return result;
}


vector<double> resolverSistema(Matrix matrix, vector<double> b) {
    //debug("Descompongo el Sistema");
    int n = matrix.n;
    std::vector<int> row_permutations;
    for (int i = 0; i < n; i++) { row_permutations.push_back(i); }
    // Descompongo usando Gauss
    for (int i = 0; i < n; ++i) {
        int pivot_row = matrix.findPivot(i,i);
        if (pivot_row != i) {
            swap(row_permutations[i], row_permutations[pivot_row]);
            matrix.swap_rows(i, pivot_row);
            swap(b[i], b[pivot_row]);
        }
        for (int j = i + 1; j < n; ++j) {
            matrix.row_operation(i, j, b);
        }
    }

    // Resuelvo el sistema

    //debug("Despejo el Sistema");

    b[n - 1] /= matrix.getElem(n - 1, n - 1);
    for (int i = n - 2; i >= 0; i--) {
        for (int j = n - 1; j > i; j--) {
            b[i] = b[i] - (matrix.getElem(i, j) * b[j]);
        }
        b[i] = b[i] / matrix.getElem(i, i);
    }

    std::vector<double> result(n, 0);
    for (int i = 0; i < n; ++i) {
        result[row_permutations[i]] = b[i];
    }

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
