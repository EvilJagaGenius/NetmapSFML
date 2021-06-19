#ifndef PROGRAMACTION_H
#define PROGRAMACTION_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>
#include <SFML/Graphics.hpp>

#include "databattlepiece.h"
#include "toolbox.h"

using namespace std;

class ProgramAction {
    public:
        // Variables
        string filename;
        string actionName;
        string description;
        int range;
        int targetSprite;  // An enum might be useful for this.  0 = red (damage), 1 = green (grid), 2 = blue (buff/debuff)
        int numOfTargets;
        vector<string> commands;  // Commands from our new action-definition files
        vector<string> prereqs;

        // Functions
        ProgramAction();
        void load(string filename);
        virtual ~ProgramAction();
        // Should we be using these functions, or should the DB have some way of getting this info directly?
        //virtual void use(Netmap_Playable* db, DataBattlePiece* source, vector<sf::Vector2i> targets);
        virtual vector<sf::Vector2i> getAimArea(sf::Vector2i origin, int targetNum);
        virtual bool checkPrereqs(DataBattlePiece* p);

    protected:

    private:
};

#endif // PROGRAMACTION_H
