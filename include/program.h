#ifndef PROGRAM_H
#define PROGRAM_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <SFML/Graphics.hpp>

#include "coord.h"
#include "programsector.h"

#include "toolbox.h"

class DataBattle;  // Used to break a circular dependency
class Program
{
    public:
        // Variables
        string programType;
        string screenName;
        string description;
        Coord spriteCoord;
        sf::Color color;
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
        void load();
        void move(Coord c, bool firstTime);  // Another thing that might be reserved in C++.  Maybe make this bool?
        void addSector(Coord c);
        void useAction(int actionIndex, Coord targetCoord);
        void switchToAiming(int actionIndex);
        void noAction();
        void takeDamage(DataBattle* level, int damage);
        void grow(int amtToGrow);
        void prepForTurn();

        Program clone();  // Look up how to write copy constructors in C++

    protected:

    private:
};

#endif // PROGRAM_H
