#ifndef _UTILS_H_
#define _UTILS_H_

struct Coord {
  int x;
  int y;
};

void bresenham(Matrix& m, Coord inicio, Coord fin);

#endif
