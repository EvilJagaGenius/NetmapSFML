#ifndef PROGRAM_H
#define PROGRAM_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iterator>
#include <unordered_map>
#include <SFML/Graphics.hpp>

#include "databattlepiece.h"
#include "programsector.h"
#include "programaction.h"

#include "toolbox.h"

using namespace std;

class DataBattle;  // Used to break a circular dependency
class ProgramAction;
class Program: public DataBattlePiece
{
    public:
        // Variables
        //string programType;
        //string name;
        //string screenName;
        //string description;
        //sf::Vector2i spriteCoord;
        //sf::Color color;
        //vector<ProgramSector*> sectors;
        //vector<ProgramAction*> actions;

        //int size;  // Is size is reserved in C++?
        //int maxSize;
        //int speed;
        //int maxSpeed;
        //int currentMove;
        //int currentActionIndex;
        //ProgramAction* currentAction;

        //char state;

        // Functions
        Program(string programType);
        Program(Program* original);  // Copy constructor
        Program(DataBattlePiece* original);  // Alt copy constructor
        virtual ~Program();
        void load();
        void move(sf::Vector2i coord, bool firstTime);  // Another thing that might be reserved in C++.  Maybe make this bool?
        void addSector(sf::Vector2i coord, int pos);
        void useAction(Netmap_Playable* level, int actionIndex, vector<sf::Vector2i> targets);
        void switchToAiming(int actionIndex);
        void noAction();
        void takeDamage(int damage);
        void amputate(sf::Vector2i coord);
        void grow(Netmap_Playable* level, int amtToGrow);
        void prepForTurn();

    protected:

    private:
};


// Constants
const static sf::Texture PROGRAM_SHEET = imgLoad("Data\\Sprites\\Programs.png");
static unordered_map<string, Program*> PROGRAM_DB = {{"Hack", new Program("Hack")},
                                                    {"Slingshot", new Program("Slingshot")},
                                                    {"DataDr", new Program("DataDr")},
                                                    {"Bug", new Program("Bug")},
                                                    {"BitMan", new Program("BitMan")},
                                                    {"Sentinel", new Program("Sentinel")}};

#endif // PROGRAM_H
