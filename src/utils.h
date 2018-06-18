#ifndef _UTILS_H_
#define _UTILS_H_

#include <set>
#include <vector>
#include "matrix.h"

using namespace std;

struct Coord {
  int x;
  int y;
  Coord(): x(0), y(0) {};
  Coord(int x, int y): x(x), y(y){};
};

bool operator<(const Coord& c1, const Coord& c2);
set<Coord> discretizarRayo(Coord inicio, Coord fin);

vector<double> cuadradosMinimos(Matrix& D,vector<double>& t);
#endif
