#ifndef SRC_ESTRUCTURAS_H_
#define SRC_ESTRUCTURAS_H_

#include <ostream>

using namespace std;

struct Coord {
  int x;
  int y;
  Coord(): x(0), y(0) {}
  Coord(int x, int y): x(x), y(y){}
};

struct Rayo {
    Coord inicio;
    Coord fin;

    Rayo() {}
    Rayo(Coord i, Coord f): inicio(i), fin(f){}
};

ostream& operator<<(ostream& os, const Coord& c);
ostream& operator<<(ostream& os, const Rayo& c);

bool operator<(const Coord& c1, const Coord& c2);
bool operator<(const Rayo& r1, const Rayo& r2);

bool operator==(const Coord& c1, const Coord& c2);
bool operator==(const Rayo& r1, const Rayo& r2);

#endif  // SRC_ESTRUCTURAS_H_
