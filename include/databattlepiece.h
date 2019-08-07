#ifndef DATABATTLEPIECE_H
#define DATABATTLEPIECE_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>
#include <SFML/Graphics.hpp>

//#include "netmap_playable.h"
#include "programsector.h"

using namespace std;

class ProgramAction;
class Netmap_Playable;
class DataBattlePiece
{
    public:
        // Variables
        string programType;
        string name;
        string screenName;
        string description;
        sf::Vector2i spriteCoord;
        sf::Color color;
        vector<ProgramSector*> sectors;
        vector<ProgramAction*> actions;

        int size;  // Is size is reserved in C++?
        int maxSize;
        int speed;
        int maxSpeed;
        int currentMove;
        ProgramAction* currentAction;
        int currentActionIndex;
        int targetCounter;
        int invisibilityTimer;
        bool visible;

        char owner;  // Who's using the program.  Player, defenders, etc.  NTS: DataBattles with multiple factions would be cool (like you + Gemma vs. WhiteKnight)
        char pieceType;  // Used to differentiate between programs/users/etc
        char state;

        // Functions
        DataBattlePiece();
        virtual ~DataBattlePiece();
        virtual void deleteSectors();
        virtual void load();
        virtual void move(sf::Vector2i coord, bool firstTime);  // Another thing that might be reserved in C++.  Maybe make this bool?
        virtual void addSector(sf::Vector2i coord, int pos);
        virtual void useAction(Netmap_Playable* level, int actionIndex, vector<sf::Vector2i> targets);
        virtual void switchToAiming(int actionIndex);
        virtual void noAction();
        virtual void takeDamage(int damage);
        virtual void amputate(sf::Vector2i coord);
        virtual void grow(Netmap_Playable* level, int amtToGrow);
        virtual void prepForTurn();

    protected:

    private:
};

#endif // DATABATTLEPIECE_H