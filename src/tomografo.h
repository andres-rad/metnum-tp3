#ifndef _TOMOGRAFO_H_
#define _TOMOGRAFO_H_

#include <vector>
#include <set>
#include "matrix.h"
#include "estructuras.h"

using namespace std;

vector<Rayo> tcPorConos(Matrix& matrix, int width=1, int step=2);

set<Coord> discretizarRayo(Rayo r);

vector<double> calcularTiempos(Matrix& img, vector<Rayo>& rayos);

#endif
