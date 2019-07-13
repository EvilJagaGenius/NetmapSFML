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
        int numOfTargets;

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

class Slash: public ProgramAction {
    public:
        Slash();
        virtual ~Slash();
        void use(Netmap_Playable* db, DataBattlePiece* source, sf::Vector2i targetCoord);
        bool checkPrereqs(DataBattlePiece* p);
};

class Stone: public ProgramAction {
    public:
        Stone();
        virtual ~Stone();
        void use(Netmap_Playable* db, DataBattlePiece* source, sf::Vector2i targetCoord);
};

class Sling1: public ProgramAction {
    public:
        Sling1();
        virtual ~Sling1();
        void use(Netmap_Playable* db, DataBattlePiece* source, sf::Vector2i targetCoord);
        //bool checkPrereqs(DataBattlePiece* p);  // Don't know if we'll need this yet
};

class Grow: public ProgramAction {
    public:
        Grow();
        virtual ~Grow();
        void use(Netmap_Playable* db, DataBattlePiece* source, sf::Vector2i targetCoord);
};

class Surgery1: public ProgramAction {
    public:
        Surgery1();
        virtual ~Surgery1();
        void use(Netmap_Playable* db, DataBattlePiece* source, sf::Vector2i targetCoord);
        bool checkPrereqs(DataBattlePiece* p);
};

class Glitch: public ProgramAction {
    public:
        Glitch();
        virtual ~Glitch();
        void use(Netmap_Playable* db, DataBattlePiece* source, sf::Vector2i targetCoord);
};

class Cut: public ProgramAction {
    public:
        Cut();
        virtual ~Cut();
        void use(Netmap_Playable* db, DataBattlePiece* source, sf::Vector2i targetCoord);
};

static unordered_map<string, ProgramAction*> ACTION_DB = {{"Slice", new Slice()},
                                                        {"Slash", new Slash()},
                                                        {"Stone", new Stone()},
                                                        {"Sling1", new Sling1()},
                                                        {"Grow", new Grow()},
                                                        {"Surgery1", new Surgery1()},
                                                        {"Glitch", new Glitch()},
                                                        {"Cut", new Cut()}};

#endif // PROGRAMACTION_H
