#include <cassert>
#include "./generadores_rayos.h"


vector<Rayo> tcPorConos(Matrix &matrix, int width, int step) {
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
    set<Rayo> tc;

    assert(n == m);
    assert(step > 0 && step <= n);
    assert(width >= 0);

    for (int base = 0; base < n; base += step) {
        for (int endX = base - width; endX <= base + width; endX++) {
            if(endX < 0){
                if(m > abs(endX)){
                    tc.insert(Rayo(Coord(base, 0), Coord(0, m - abs(endX))));
                }
            } else if(endX > m){
                if(2*m - endX > 0){
                    tc.insert(Rayo(Coord(base, 0), Coord(n-1, 2*m - endX)));
                }
            } else {
                tc.insert(Rayo(Coord(base, 0), Coord(endX, m)));
                tc.insert(Rayo(Coord(endX, 0), Coord(base, m)));
            }

        }
    }

    for (int base = 0; base < m; base += step) {
        for (int endY = max(0, base - width); endY <= min(n, base + width); endY++) {
            tc.insert(Rayo(Coord(0, base), Coord(n, endY)));
            tc.insert(Rayo(Coord(0, endY), Coord(n, base)));
        }
    }
    vector<Rayo> tcv;
    for(auto r : tc) tcv.push_back(r);
    return tcv;
}

vector<Rayo> tCicrular(Matrix &matrix, bool completa) {
    int n = matrix.dimensions().first - 1;
    int m = matrix.dimensions().second - 1;
    int fin = n - 1;
    assert(n == m);
    vector<Rayo> tcir;
    tcir.push_back(Rayo(Coord(0, 0), Coord(fin, fin)));
    for (int i = 1; i < n; i++) {
        tcir.push_back(Rayo(Coord(0, i), Coord(fin, fin - i)));
        tcir.push_back(Rayo(Coord(i, 0), Coord(fin - i, fin)));
    }
    if (completa) {
        tcir.push_back(Rayo(Coord(fin, fin), Coord(0, 0)));
        for (int i = 1; i < n; i++) {
            tcir.push_back(Rayo(Coord(fin, fin - i), Coord(0, i)));
            tcir.push_back(Rayo(Coord(fin - i, fin), Coord(i, 0)));
        }
    }
    return tcir;
}

vector<Rayo> tRecta(Matrix &matrix, bool completa) {
    int n = matrix.dimensions().first - 1;
    int m = matrix.dimensions().second - 1;
    int fin = n - 1;
    assert(n == m);
    vector<Rayo> trec;
    for (int i = 0; i < n; i++) {
        trec.push_back(Rayo(Coord(0, i), Coord(fin, i)));
        trec.push_back(Rayo(Coord(i, 0), Coord(i, fin)));
    }
    if (completa) {
        for (int i = 0; i < n; i++) {
            trec.push_back(Rayo(Coord(fin, i), Coord(0, i)));
            trec.push_back(Rayo(Coord(i, fin), Coord(i, 0)));
        }
    }
    return trec;
}

set<Coord> coordenadasDeRayo(Rayo r) {
    // Sacado de http://eugen.dedu.free.fr/projects/bresenham/
    set<Coord> res;
    Coord inicio = r.inicio;
    Coord fin = r.fin;

    int deltaX = fin.x - inicio.x;
    int deltaY = fin.y - inicio.y;
    int curY = inicio.y;
    int curX = inicio.x;
    res.insert(inicio);

    int stepX = 1;
    int stepY = 1;
    int ultError, error;

    if (deltaX < 0) {
        stepX = -1;
        deltaX = -deltaX;
    }
    if (deltaY < 0) {
        stepY = -1;
        deltaY = -deltaY;
    }

    int dobleDX = 2 * deltaX;
    int dobleDY = 2 * deltaY;

    if (dobleDX >= dobleDY) {
        // Pendiente menor a identidad
        ultError = error = deltaX;

        for (int i = 0; i < deltaX; i++) {
            curX += stepX;
            error += dobleDY;

            if (error > dobleDX) {
                curY += stepY;
                error -= dobleDX;
            }

            if (error + ultError < dobleDX) {
                res.insert(Coord(curX, curY - stepY));
            } else if (error + ultError > dobleDX) {
                res.insert(Coord(curX - stepX, curY));
            }
            res.insert(Coord(curX, curY));
            ultError = error;
        }
    } else {  // Pendiente mayor a identidad
        ultError = error = deltaY;
        for (int i = 0; i < deltaY; i++) {
            curY += stepY;
            error += dobleDX;

            if (error > dobleDY) {
                curX += stepX;
                error -= dobleDY;
                if (error + ultError < dobleDY) {
                    res.insert(Coord(curX - stepX, curY));
                } else if (error + ultError > dobleDY) {
                    res.insert(Coord(curX, curY - stepY));
                }
            }
            res.insert(Coord(curX, curY));
            ultError = error;
        }
    }
    return res;
}
