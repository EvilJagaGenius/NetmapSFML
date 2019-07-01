#ifndef DATABATTLE_H
#define DATABATTLE_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>
#include <SFML/Graphics.hpp>

#include "netmap_playable.h"
#include "toolbox.h"
#include "program.h"
#include "inputbox.h"
#include "player.h"

using namespace std;

const static int TILE_SIZE = 32;
const static int GAP_SIZE = 4;

class DataBattle: public Netmap_Playable
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
        int selectedUpload;
        unordered_map<string, Program*> defenders;
        vector<Program*> friendlies;
        int grid[16][16];

        // play()-specific variables here
        sf::Texture gridSheet;
        sf::Sprite gridSprite;
        sf::Sprite programSprite;
        sf::Vector2<int> mousePos;
        char phase;
        string lookingAt;
        InputBox* hud;
        Player* player;
        int friendliesLoaded;
        int currentProgramIndex;
        Program* currentProgram;
        Program* programStartingState; // Write a copy constructor for Program
        string currentDefenderIndex;
        sf::Vector2i programHead;
        sf::Vector2i nButton;
        sf::Vector2i sButton;
        sf::Vector2i eButton;
        sf::Vector2i wButton;
        vector<sf::Vector2i> moveArea;

        // Functions
        DataBattle();
        DataBattle(string filename);
        virtual ~DataBattle();
        void load();
        void render(sf::RenderWindow* window);
        void setHUD(InputBox* hud);
        void setPlayer(Player* player);
        void play(sf::RenderWindow* window);
        void switchTurns(InputBox* hud);
        string takeCommand(string command);
        string lookAt(sf::Vector2i coord);

    protected:

    private:
};

const static sf::Font DEFAULT_FONT = fontLoad("Data\\Fonts\\Terminus.ttf");  // I needed someplace to put this
const static sf::Texture GRID_SHEET = imgLoad("Data\\Sprites\\Grid.png");

#endif // DATABATTLE_H
