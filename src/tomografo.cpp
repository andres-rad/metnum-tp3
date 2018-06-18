#include <vector>
#include <utility>
#include "tomografo.h"
#include "estructuras.h"
#include <cassert>

//Prototipos
set<Coord> discretizarRayo(Rayo r);


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

vector<double> calcularTiempos(Matrix& img, vector<Rayo>& rayos){
    /* Dada una imagen y el conjunto de rayos de la tomografia
     * calcula el tiempo que tarda el recorrido de cada uno de los rayos
     *
     * IN:
     *  img -> imagen a la cual se le hace la tomografia
     *  rayos -> conjunto de rayos generados por la tomografia
     *
     * OUT:
     *  tiempos -> tiempo que tardo el recorrido de cada rayo
     */

    vector<double> tiempos;
    for(auto r : rayos){
        for(auto celda : discretizarRayo(r)){
            tiempos.push_back(img[celda.x][celda.y]);
        }
    }
    return tiempos;
}

set<Coord> discretizarRayo(Rayo r){
    //Sacado de http://eugen.dedu.free.fr/projects/bresenham/
    set<Coord> res;
    Coord inicio = r.inicio;
    Coord fin = r.fin;

    int deltaX = fin.x - inicio.x;
    int deltaY = fin.y - inicio.y;
    int dobleDX = 2 * deltaX;
    int dobleDY = 2 * deltaY;
    int curY = inicio.y;
    int curX = inicio.x;
    int stepX = 1;
    int stepY = 1;
    int ultError, error;

    if(deltaX < 0) {
        stepX = -1;
        deltaX = -deltaX;
    }
    if(deltaY < 0) {
        stepY = -1;
        deltaY = -deltaY;
    }

    if(dobleDX >= dobleDY){ //Pendiente menor a identidad
        ultError = error = deltaX;

        for(int i = 0; i < deltaX; i++){
            curX += stepX;
            error += dobleDY;

            if(error > dobleDX){
                curY += stepY;
                error -= dobleDX;
            }

            if(error + ultError < dobleDX){
                res.insert(Coord(curX, curY-stepY));
            } else if (error + ultError > 2 * deltaX){
                res.insert(Coord(curX - stepX, curY));
            }
            res.insert(Coord(curX, curY));
            ultError = error;
        }
    } else { //Pendiente mayor a identidad
        ultError = error = deltaY;
        for(int i = 0; i < deltaY; i++){
            curY += stepY;
            error += 2 * deltaX;

            if(error > 2 * deltaY){
                    curX += stepX;
                    error -= 2 * deltaY;
                    if(error + ultError < 2 * deltaY){
                        res.insert(Coord(curX - stepX, curY));
                    } else if (error + ultError > 2 * deltaY){
                        res.insert(Coord(curX, curY - stepY));
                    }
            }
            res.insert(Coord(curX, curY));
            ultError = error;
        }
    }
    return res;
}
