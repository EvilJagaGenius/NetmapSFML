#include "programsector.h"

ProgramSector::ProgramSector(Coord c) {
    this->coord = c;
    this->numLinks = 0;
}

ProgramSector::ProgramSector(Coord c, ProgramSector lastSector) {
    this->coord = c;
    this->numLinks = 1;
    this->links.push_back(lastSector);
}

ProgramSector::~ProgramSector() {
    //dtor
}
