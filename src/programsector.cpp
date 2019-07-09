#include "programsector.h"

ProgramSector::ProgramSector(sf::Vector2i coord) {
    this->coord = coord;
    this->numLinks = 0;
}

ProgramSector::ProgramSector(sf::Vector2i coord, ProgramSector* lastSector) {
    this->coord = coord;
    this->numLinks = 0;
    linkSectors(this, lastSector);
    //cout << this->numLinks << '\n';
    //cout << lastSector->numLinks << '\n';
}

ProgramSector::~ProgramSector() {
    //dtor
}

void ProgramSector::linkSectors(ProgramSector* a, ProgramSector* b) {
    a->links.push_back(b);
    a->numLinks++;
    b->links.push_back(a);
    b->numLinks++;
}
