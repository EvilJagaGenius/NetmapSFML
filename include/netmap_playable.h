// A class that all playable objects (databattles, scenes, etc) in Netmap 1.0 inherit from
#ifndef NETMAP_PLAYABLE_H
#define NETMAP_PLAYABLE_H

#include <iostream>
#include <unordered_map>
#include <string>
#include <SFML/Graphics.hpp>

#include "databattlepiece.h"

using namespace std;

class InputBox;
class Player;
class Program;
class Netmap_Playable
{
    public:
        char playableType;
        unordered_map<string, DataBattlePiece*> defenders;
        vector<DataBattlePiece*> friendlies;

        Netmap_Playable();
        virtual ~Netmap_Playable();
        virtual void render(sf::RenderWindow* window);
        virtual string play(sf::RenderWindow* window);
        virtual void setHUD(InputBox* hud);
        virtual void setPlayer(Player* player);
        virtual string takeCommand(string command);
        virtual string lookAt(sf::Vector2i coord);
        virtual void flipSector(sf::Vector2i coord);

    protected:

    private:
};

#endif // NETMAP_PLAYABLE_H
