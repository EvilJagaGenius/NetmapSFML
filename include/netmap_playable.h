// A class that all playable objects (databattles, scenes, etc) in Netmap 1.0 inherit from
#ifndef NETMAP_PLAYABLE_H
#define NETMAP_PLAYABLE_H

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>

using namespace std;

class InputBox;
class Player;
class Netmap_Playable
{
    public:
        char playableType;

        Netmap_Playable();
        virtual ~Netmap_Playable();
        virtual void render(sf::RenderWindow* window);
        virtual void play(sf::RenderWindow* window);
        virtual void setHUD(InputBox* hud);
        virtual void setPlayer(Player* player);
        virtual string takeCommand(string command);

    protected:

    private:
};

#endif // NETMAP_PLAYABLE_H
