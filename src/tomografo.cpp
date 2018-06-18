#include <vector>
#include <utility>
#include "tomografo.h"
#include "estructuras.h"
#include <cassert>

vector<Rayo> tcPorConos(Matrix& matrix, int width, int step){
    /* Funcion que simula el conjunto de rayos que genera una tomografia.
     * El emisor se mueve por los lados de la imagen y tira rayos en
     * forma de cono a cada paso.
     *
     * IN:
     *  matrix -> imagen a la cual se le hace la tomografia
     *  width  -> ancho del cono de rayos generado en cada paso
     *     donde ancho = distancia del centro del arco del cono hasta una punta
     *  step   -> cuanto avanza el emisor de rayos en cada paso
     *
     * OUT:
     *  res -> rayos emitidos sobre la imagen dados los
     *         parametros de entrada
     */
    int n = matrix.dimensions().first - 1;
    int m = matrix.dimensions().second - 1;
    vector<Rayo> tc;

    assert(n==m);
    assert(step > 0 && step <= n);
    assert(width >= 0);

    for(int base = 0; base < n; base += step){
        for(int endX = max(0, base-width); endX <= min(m, base+width); endX++){
            tc.push_back(Rayo(Coord(base, 0), Coord(endX, m)));
            tc.push_back(Rayo(Coord(base, m), Coord(endX, 0)));
        }
    }

    for(int base = 0; base < m; base+= step){
        for(int endY = max(0, base-width); endY <= min(n, base+width); endY++){
            tc.push_back(Rayo(Coord(0, base), Coord(n, endY)));
            tc.push_back(Rayo(Coord(n, base), Coord(0, endY)));
        }
    }

    return tc;
}
