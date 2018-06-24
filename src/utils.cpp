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

//vector<double> randomVector(int i) {
//    vector<double> vector(i, 0);
//    random_device rd;
//    mt19937 mt(rd());
//    uniform_real_distribution<double> dist(-10.0, 10.0);
//    for (int j = 0; j < i; ++j) {
//        vector[j] = dist(mt) / 10;
//    }
//    normalize(vector);
//    return vector;
//}

//vector<double> gaussSeidel(Matrix matrix, std::vector<double> b) {
//    vector<double> x = randomVector(matrix.n);
//
//    for (int iter = 0; iter < MAXITER; iter++) {
//        for (int j = 0; j < matrix.n; j++) {
//            double sum = 0;
//            for (int i = 0; i < matrix.n; i++) {
//                if (i != j) { sum += matrix[j][i] * x[i]; }
//            }
//            x[j] = (b[j] - sum) / matrix[j][j];
//        }
//        // debugVec(x);
//    }
//    return x;
//}

vector<double> cuadradosMinimos(Matrix &D, vector<double> &t) {
    Matrix transposedD = D.transpose();
    Matrix A = transposedD * D;

    vector<double> b = transposedD * t;
    vector<double> result = resolverSistema(A, b);

    return result;
}

int findPivot(Matrix &matrix, int k) {
    int pivot = k;
    double pivot_value = 0;
    for (int i = k; i < matrix.n; i++) {
        if (abs(matrix.getElem(i, k)) > abs(pivot_value)) {
            pivot = i;
            pivot_value = matrix.getElem(i, k);
        }
    }
    return pivot;
}

vector<double> resolverSistema(Matrix matrix, vector<double> b) {
    debug("Resuelvo el Sistema");
    cout << matrix << endl;
    int n = matrix.n;
    std::vector<int> row_permutations;
    for (int i = 0; i < n; i++) { row_permutations.push_back(i); }
    std::vector<int> col_permutations;
    for (int i = 0; i < n; i++) { col_permutations.push_back(i); }
    // Descompongo usando Gauss
    for (int i = 0; i < n; ++i) {
        int pivot_col = i;
        int pivot_row = i;
        double pivot_value = 0;
        for (int k = i; k < n; ++k) {
            for (int j = i; j < n; ++j) {
                double temp = matrix.getElem(i, j);
                if (abs(temp) > pivot_value) {
                    pivot_row = i;
                    pivot_col = j;
                    pivot_value = temp;
                }
            }
        }
        if (pivot_row != i) {
            swap(row_permutations[i], row_permutations[pivot_row]);
            matrix.swap_rows(i, pivot_col);
            swap(b[i], b[pivot_row]);
        }
        if (pivot_col != i) {
            swap(col_permutations[i], col_permutations[pivot_col]);
            for (int l = 0; l < n; ++l) {
                double swap = matrix.getElem(l, i);
                matrix.setElem(l, i, matrix.getElem(l, pivot_col));
                matrix.setElem(l, pivot_col, swap);
            }
        }

        for (int j = i + 1; j < n; ++j) {
            matrix.row_operation(i, j, b);
        }
    }

    // Resuelvo el sistema
    debug("Terminando EG");
    cout << matrix << endl;


    b[n - 1] /= matrix.getElem(n - 1, n - 1);
    for (int i = n - 2; i >= 0; i--) {
        for (int j = n - 1; j > i; j--) {
            b[i] = b[i] - (matrix.getElem(i, j) * b[j]);
        }
        b[i] = b[i] / matrix.getElem(i, i);
    }

    std::vector<double> result(n, 0);
    for (int i = 0; i < n; ++i) {
        int temp = row_permutations[i];
        result[col_permutations[temp]] = b[i];
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
