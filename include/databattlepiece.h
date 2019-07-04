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
class DataBattle;
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
        vector<ProgramSector> sectors;
        vector<ProgramAction*> actions;

        int size;  // Is size is reserved in C++?
        int maxSize;
        int speed;
        int maxSpeed;
        int currentMove;

        char state;

        // Functions
        DataBattlePiece();
        virtual ~DataBattlePiece();
        void load();
        void move(sf::Vector2i coord, bool firstTime);  // Another thing that might be reserved in C++.  Maybe make this bool?
        void addSector(sf::Vector2i coord, int pos);
        void useAction(DataBattle* level, int actionIndex, sf::Vector2i targetCoord);
        void switchToAiming(int actionIndex);
        void noAction();
        void takeDamage(int damage);
        void grow(DataBattle* level, int amtToGrow);
        void prepForTurn();

    protected:

    private:
};

#endif // DATABATTLEPIECE_H
