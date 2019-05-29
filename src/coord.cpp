#include "coord.h"

Coord::Coord() {
    this->x = 0;
    this->y = 0;
}

Coord::Coord(int x, int y)
{
    this->x = x;
    this->y = y;
}

Coord::~Coord()
{
    //dtor
}
