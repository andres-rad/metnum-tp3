#ifndef _UTILS_H_
#define _UTILS_H_

#include <vector>
#include "matrix.h"

using namespace std;

struct Coord {
  int x;
  int y;
};

vector<Coord> discretizarRayo(Matrix& m, Coord inicio, Coord fin);

vector<double> cuadradosMinimos(Matrix& D,vector<double>& t);
#endif
