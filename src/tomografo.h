#ifndef _TOMOGRAFO_H_
#define _TOMOGRAFO_H_

#include <vector>
#include "matrix.h"
#include "utils.h"

using namespace std;

vector<Rayo> tcPorConos(Matrix& matrix, int width=1, int step=2);

#endif
