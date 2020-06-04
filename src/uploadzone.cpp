#include "uploadzone.h"

UploadZone::UploadZone() {
    //ctor
}

UploadZone::UploadZone(int x, int y, int owner) {
    this->owner = owner;
    this->pieceType = 'u';
    // I'm not sure whether we should use ProgramSectors or not.  I guess I'll use them for consistency's sake
    this->sectors.push_back(new ProgramSector(this->headCoord));
}

UploadZone::~UploadZone() {
    //dtor
}
