#include "utils.h"
#include "matrix.h"
#include <vector>
#include <iostream>
#include <utility>

vector<Coord> lineaConPendienteBaja(Matrix& m, Coord inicio, Coord fin);
vector<Coord> lineaConPendienteElevada(Matrix& m, Coord inicio, Coord fin);
bool algunoFueraDeRango(Matrix& m, int x, int y);


vector<Coord> discretizarRayo(Matrix& m, Coord inicio, Coord fin){
    /* Implementacion del algoritmo de Bresenham: dada una matriz, una
     * coordenada inicial y una final retorna un vector con las coordenadas
     * que atraviesa la recta
    */
    cerr << "discretizando" << endl;

    int x0 = inicio.x; int xf = fin.x;
    int y0 = inicio.y; int yf = fin.y;

    cerr << x0 << " " << xf << " " << y0 << " " << yf << endl;

    if(abs(yf - y0) <= abs(xf -  x0)){
        //Si la recta es creciente o decreciente alcanza con invertir ejes.
        if(x0 < xf) {
            return lineaConPendienteBaja(m, inicio, fin);
        } else {
            return lineaConPendienteBaja(m, fin, inicio);
        }
    } else {
        if(y0 < yf) {
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
    cout << "Pendiente baja" << endl;
    cout << inicio.x << " " << inicio.y << endl;

    int deltaX = fin.x - inicio.x;
    int deltaY = fin.y - inicio.y;
    int diff = 2 * deltaY - deltaX;
    int curY = inicio.y;
    int stepY;
    vector<Coord> res;

    if(deltaY >= 0){
        stepY = 1;
    } else {
        stepY = -1;
        deltaY = -deltaY;
    }

    Coord temp;
    for(int curX = inicio.x; curX <= fin.x; curX++){
        
        if(algunoFueraDeRango(m, curX, curY)) {
            cout << "Fuera de rango" << endl;
            break;
        }

        temp.x = curX; temp.y = curY;

        res.push_back(temp);

        if(diff > 0){
            curY += stepY;
            diff -= 2 * deltaX;
        }
        diff += 2 * deltaY;
    }

    return res;
}

vector<Coord> lineaConPendienteElevada(Matrix& m, Coord inicio, Coord fin){
    /* Setea la matriz para el caso en que la linea tiene pendiente de
     * modulo > 1.
     */
    int deltaX = fin.x - inicio.x;
    int deltaY = fin.y - inicio.y;
    int curX = inicio.x;
    int stepX;
    vector<Coord> res;

    if(deltaX >= 0){
        stepX = 1;
    } else {
        stepX = -1;
        deltaX = -deltaX;
    }

    int diff = 2 * deltaX - deltaY;

    Coord temp;

    for(int curY = inicio.y; curY <= fin.y; curY++){

        if(algunoFueraDeRango(m, curX, curY)) break;

        temp.x = curX; temp.y = curY;
        res.push_back(temp);
        
        if(diff > 0){
            curX += stepX;
            diff += 2 * deltaX;
        }
    }

    return res;
}

bool algunoFueraDeRango(Matrix& m, int x, int y){
    int xLim = m.dimensions().first;
    int yLim = m.dimensions().second;

    return (x < 0 || x > xLim || y < 0 || y > yLim);
}
