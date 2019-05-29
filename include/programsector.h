#ifndef PROGRAMSECTOR_H
#define PROGRAMSECTOR_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <SFML/Graphics.hpp>

#include "toolbox.h"
#include "coord.h"

class ProgramSector
{
    public:
        Coord coord;
        vector<ProgramSector> links;
        int numLinks;

        ProgramSector(Coord c);
        ProgramSector(Coord c, ProgramSector lastSector);
        virtual ~ProgramSector();

    protected:

    private:
};

#endif // PROGRAMSECTOR_H
