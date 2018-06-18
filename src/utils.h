#ifndef _UTILS_H_
#define _UTILS_H_

#include <set>
#include <vector>
#include "matrix.h"
#include "estructuras.h"

using namespace std;

set<Coord> discretizarRayo(Coord inicio, Coord fin);

vector<double> cuadradosMinimos(Matrix& D,vector<double>& t);

#endif
