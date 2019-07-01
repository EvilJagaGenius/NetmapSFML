#ifndef PROGRAM_H
#define PROGRAM_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iterator>
#include <unordered_map>
#include <SFML/Graphics.hpp>

#include "programsector.h"

#include "toolbox.h"

using namespace std;

class DataBattle;  // Used to break a circular dependency
class Program
{
    public:
        // Variables
        string programType;
        string name;
        string screenName;
        string description;
        sf::Vector2i spriteCoord;
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
        Program(Program* original);  // Copy constructor
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


// Constants
const static sf::Texture PROGRAM_SHEET = imgLoad("Data\\Sprites\\Programs.png");
static unordered_map<string, Program*> PROGRAM_DB = {{"Hack", new Program("Hack")},
                                                    {"Slingshot", new Program("Slingshot")}};

#endif // PROGRAM_H
