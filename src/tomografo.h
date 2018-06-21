#ifndef SRC_TOMOGRAFO_H_
#define SRC_TOMOGRAFO_H_

#include <vector>
#include <set>
#include "./matrix.h"
#include "./estructuras.h"

using namespace std;

vector<Rayo> tcPorConos(Matrix& matrix, int width = 1, int step = 2);

set<Coord> coordenadasDeRayo(Rayo r);

vector<double> calcularTiempos(Matrix& img, vector<Rayo>& rayos);

Matrix generarDiscretizacion(const Matrix& img_original, const vector<Rayo>& rayos, int magnitud_discretizacion);

Coord pixel_real_a_discretizado(Coord real, int magnitud_discretizacion);

#endif  // SRC_TOMOGRAFO_H_
