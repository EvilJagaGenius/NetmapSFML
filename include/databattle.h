#ifndef DATABATTLE_H
#define DATABATTLE_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>
#include <SFML/Graphics.hpp>

#include "toolbox.h"
#include "program.h"
#include "hud.h"

using namespace std;

// Constants
const static int TILE_SIZE = 32;
const static int GAP_SIZE = 4;

class HUD;
class DataBattle
{
    // Following my style, make everything public until needed otherwise
    public:
        // Variables
        string filename;
        sf::Texture bkg;
        sf::Sprite bkgSprite;
        // Insert music file here
        int cashReward;
        vector<sf::Vector2i> cashPickups;  // A list of coordinates
        vector<sf::Vector2i> uploads;
        unordered_map<string, Program*> defenders;
        int grid[16][16];

        // play()-specific variables here
        sf::Texture gridSheet;
        sf::Sprite gridSprite;
        sf::Sprite programSprite;
        sf::Vector2<int> mousePos;
        char phase;

        // Functions
        DataBattle();
        DataBattle(string filename);
        virtual ~DataBattle();
        void load();
        void render(sf::RenderWindow* window);
        void play(sf::RenderWindow* window, HUD* hud);
        void takeCommand(string command);

    protected:

    private:
};

#endif // DATABATTLE_H
