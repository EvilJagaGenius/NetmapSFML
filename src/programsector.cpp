#include "programsector.h"

ProgramSector::ProgramSector(sf::Vector2i coord) {
    this->coord = coord;
    this->numLinks = 0;
}

ProgramSector::ProgramSector(sf::Vector2i coord, ProgramSector lastSector) {
    this->coord = coord;
    this->numLinks = 1;
    this->links.push_back(lastSector);
}

ProgramSector::~ProgramSector() {
    //dtor
}
