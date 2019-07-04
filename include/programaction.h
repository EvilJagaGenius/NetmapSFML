#ifndef PROGRAMACTION_H
#define PROGRAMACTION_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>
#include <SFML/Graphics.hpp>

#include "netmap_playable.h"
#include "databattlepiece.h"
#include "toolbox.h"

using namespace std;

class ProgramAction {
    public:
        // Variables
        string actionName;
        string description;
        int range;
        int targetSprite;  // An enum might be useful for this.  0 = red (damage), 1 = green (grid), 2 = blue (buff/debuff)

        // Functions
        ProgramAction();
        virtual ~ProgramAction();
        virtual void use(Netmap_Playable* db, DataBattlePiece* source, sf::Vector2i targetCoord);
        virtual vector<sf::Vector2i> getAimArea(sf::Vector2i origin);
        virtual bool checkPrereqs(DataBattlePiece* p);

    protected:

    private:
};

// Child classes
class Slice: public ProgramAction {
    public:
        Slice();
        virtual ~Slice();
        void use(Netmap_Playable* db, DataBattlePiece* source, sf::Vector2i targetCoord);
};

class Stone: public ProgramAction {
    public:
        Stone();
        virtual ~Stone();
        void use(Netmap_Playable* db, DataBattlePiece* source, sf::Vector2i targetCoord);
};

class Cut: public ProgramAction {
    public:
        Cut();
        virtual ~Cut();
        void use(Netmap_Playable* db, DataBattlePiece* source, sf::Vector2i targetCoord);
};

static unordered_map<string, ProgramAction> ACTION_DB = {{"Slice", Slice()},
                                                        {"Stone", Stone()},
                                                        {"Cut", Cut()}};

#endif // PROGRAMACTION_H
