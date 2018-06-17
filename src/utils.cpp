#include "utils.h"
#include <set>
#include <iostream>
#include <utility>

set<Coord> discretizarRayo(Coord inicio, Coord fin){
    //Sacado de http://eugen.dedu.free.fr/projects/bresenham/
    set<Coord> res;

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

bool operator<(const Coord& c1, const Coord& c2){
	return tie(c1.x,c1.y) < tie(c2.x, c2.y);
}
