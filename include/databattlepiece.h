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

        int owner;  // Whose inventory this piece gets returned to upon deletion
        int controller;  // Who is currently controlling the piece
        char pieceType;  // Used to differentiate between programs/users/etc
        char state;

        // Experimental AI variables below
        int targetPlayer;
        int targetTeam;
        DataBattlePiece* targetPiece;
        sf::Vector2i targetCoord;
        char orders;  // Attack, buff, heal, etc.
        // End AI variables

        unordered_map<char, int> statuses; // Stuff like invisibility, poison, etc. goes here

        // Functions
        DataBattlePiece();
        virtual ~DataBattlePiece();
        virtual void deleteSectors();
        virtual void load();
        virtual void move(Netmap_Playable* level, sf::Vector2i coord, bool firstTime);  // Another thing that might be reserved in C++.  Maybe make this bool?
        virtual void addSector(sf::Vector2i coord, int pos);
        virtual void useAction(Netmap_Playable* level, int actionIndex, vector<sf::Vector2i> targets);
        virtual void switchToAiming(int actionIndex);
        virtual void noAction();
        virtual void takeDamage(Netmap_Playable* level, int damage);
        virtual void amputate(Netmap_Playable* level, sf::Vector2i coord);
        virtual void grow(Netmap_Playable* level, int amtToGrow);
        virtual void prepForTurn();

    protected:

    private:
};

#endif // DATABATTLEPIECE_H
