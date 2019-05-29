#ifndef PROGRAM_H
#define PROGRAM_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <SFML/Graphics.hpp>

#include "toolbox.h"
#include "coord.h"
#include "programsector.h"

class Program
{
    public:
        // Variables
        string programType;
        string screenName;
        string description;
        Coord spriteCoord;
        vector<ProgramSector> sectors;

        int size;  // Is size is reserved in C++?
        int maxSize;
        int speed;
        int maxSpeed;
        int currentMove;

        char state;

        // Functions
        Program(string programType);
        virtual ~Program();

    protected:

    private:
};

#endif // PROGRAM_H
