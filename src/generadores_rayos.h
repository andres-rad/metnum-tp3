#ifndef SRC_GENERADORES_RAYOS_H_
#define SRC_GENERADORES_RAYOS_H_

#include <vector>
#include "./matrix.h"
#include "./estructuras.h"

using namespace std;

vector<Rayo> tcPorConos(Matrix& matrix, int width = 1, int step = 2);

vector<Rayo> tCicrular(Matrix& matrix, bool completa);

vector<Rayo> tRecta(Matrix& matrix, bool completa);

vector<Rayo> tcRandom(const Matrix &matrix, uint cant_rayos);

set<Coord> coordenadasDeRayo(Rayo r);

#endif  // SRC_GENERADORES_RAYOS_H_
