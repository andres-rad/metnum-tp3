#ifndef SRC_UTILS_H_
#define SRC_UTILS_H_

#include <set>
#include <vector>
#include "matrix.h"
#include "estructuras.h"

using namespace std;

vector<double> cuadradosMinimos(Matrix& D, vector<double>& t);

void agregarRuido(vector<double>& t, int tipo, double porcentaje, double max, double std);

void guaussianNoise(vector<double>& t, int cantidad, int positions[], double max, double std);

void poissonNoise(vector<double>& t, int cantidad, int positions[], double max);

void randomNoise(vector<double>& t, int cantidad, int positions[], double max);

void outlierNoise(vector<double>& t, int cantidad, int positions[], double max);

#endif  // SRC_UTILS_H_
