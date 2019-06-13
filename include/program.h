#ifndef PROGRAM_H
#define PROGRAM_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iterator>
#include <SFML/Graphics.hpp>

#include "programsector.h"

#include "toolbox.h"

// Constants
const static sf::Texture PROGRAM_SHEET = imgLoad("Data\\Sprites\\Programs.png");

class DataBattle;  // Used to break a circular dependency
class Program
{
    public:
        // Variables
        string programType;
        string screenName;
        string description;
        sf::Vector2i spriteCoord;
        sf::Color color;
        vector<ProgramSector> sectors;  // As opposed to the original Netmap code, let's make the head the back of this vector

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
        void move(sf::Vector2i coord, bool firstTime);  // Another thing that might be reserved in C++.  Maybe make this bool?
        void addSector(sf::Vector2i coord, int pos);
        void useAction(DataBattle* level, int actionIndex, sf::Vector2i targetCoord);
        void switchToAiming(int actionIndex);
        void noAction();
        void takeDamage(DataBattle* level, int damage);
        void grow(DataBattle* level, int amtToGrow);
        void prepForTurn();

        Program clone();  // Look up how to write copy constructors in C++

    protected:

    private:
};

#endif // PROGRAM_H
