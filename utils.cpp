#include <utils.h>


vector<Coord> discretizarRayo(Matrix& m, Coord inicio, Coord fin){
    /* Implementacion del algoritmo de Bresenham: dada una matriz, una
     * coordenada inicial y una final retorna un vector con las coordenadas
     * que atraviesa la recta
    */
    x0 = inicio.x; xf = fin.x;
    y0 = inicio.y; yf = fin.y;

    if(abs(yf - y0) < abs(xf - x0)){
        //Si la recta es creciente o decreciente alcanza con invertir ejes.
        f(x0 > xf) {
            return lineaConPendienteBaja(m, inicio, fin);
        } else {
            return lineaConPendienteBaja(m, fin, inicio);
        }
    } else {
        if(y0 > yf) {
            return lineaConPendienteElevada(m, inicio, fin);
        } else {
            return lineaConPendienteElevada(m, fin, inicio);
        }
    }
}

vector<Coord> lineaConPendienteBaja(Matrix& m, Coord inicio, Coord fin){
    /* Setea la matriz para el caso en que la linea tiene pendiente de
     * modulo < 1.
     */
    int deltaX = fin.x - inicio.x;
    int deltaY = fin.y - inicio.y;
    int diff = 2 * deltaY - deltaX;
    int curY = inicio.y;
    int stepY;

    if(deltaY >= 0){
        stepY = 1;
    } else {
        stepY = -1;
        deltaY = -deltaY;
    }

    for(int curX = inicio.x; cur < fin.x; cur++){
        if(algunoFueraDeRango(m, curX, curY)) break;

        m[curX][curY] = true;
        if(diff > 0){
            curY += stepY;
            diff -= 2 * deltaX;
        }
        diff += 2 * deltaY;
    }
}

vector<Coord> lineaConPendienteElevada(Matrix& m, Coord inicio, Coord fin){
    /* Setea la matriz para el caso en que la linea tiene pendiente de
     * modulo > 1.
     */
    int deltaX = fin.x - inicio.x;
    int deltaY = fin.y - inicio.y;
    int curX = inicio.x;
    int stepX;

    if(deltaX >= 0){
        stepX = 1;
    } else {
        stepX = -1;
        deltaX = -deltaX;
    }

    int diff = 2 * deltaX - deltaY;

    for(int curY = inicio.y; curY < fin.y; curY++){

        if(algunoFueraDeRango(m, curX, curY)) break;

        m[curX][curY] = true;
        if(diff > 0){
            curX += stepX;
            diff += 2 * deltaX;
        }
    }
}

bool algunoFueraDeRango(Matrix& m, int x, int y){
    xLim = m.dimensions().first;
    yLim = m.dimensions().second;

    return x < 0 || x > xLim || y < 0 || y > yLim;
}
