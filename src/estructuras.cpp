#include <tuple>
#include "estructuras.h"
//Operadores para cout

ostream& operator<<(ostream& os, const Coord& c){
    os << "{" << c.x << ", " << c.y << "}";
    return os;
}

ostream& operator<<(ostream& os, const Rayo& r){
    os << "{ " << r.inicio << ", " << r.fin << " }";
    return os;
}


bool operator<(const Coord& c1, const Coord& c2){
	return tie(c1.x,c1.y) < tie(c2.x, c2.y);
}

bool operator<(const Rayo& r1, const Rayo& r2){
    if(r1.inicio == r2.inicio){
        return r1.fin < r2.fin;
    } else {
        return r1.inicio < r2.inicio;
    }
}

bool operator==(const Coord& c1, const Coord& c2){
    return (c1.x == c2.x) && (c1.y == c2.y);
}

bool operator==(const Rayo& r1, const Rayo& r2){
    return (r1.inicio == r2.inicio) && (r1.fin == r2.fin);
}
