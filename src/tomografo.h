#ifndef SRC_TOMOGRAFO_H_
#define SRC_TOMOGRAFO_H_

#include <vector>
#include <set>
#include "./matrix.h"
#include "./estructuras.h"

using namespace std;

vector<double> calcularTiempos(Matrix& img, vector<Rayo>& rayos);

Matrix generarDiscretizacion(const Matrix& img_original, const vector<Rayo>& rayos, int magnitud_discretizacion);

Coord pixel_real_a_discretizado(Coord real, int magnitud_discretizacion);

Matrix obtenerResultado(Matrix& img_original, int magnitud_discretizacion, int tipo_rayo,
                        int width_rayos, int step_rayos, int step_other_side, double varianza_ruido,
                        int n_rayos, int pixel_size);

#endif  // SRC_TOMOGRAFO_H_
