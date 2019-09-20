#ifndef SCENE_H
#define SCENE_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "toolbox.h"
#include "netmap_playable.h"
#include "scenelayer.h"
#include "inputbox.h"

using namespace std;

class Scene: public Netmap_Playable
{
    public:
        string filename;
        InputBox* hud;
        Player* player;
        sf::Vector2i mousePos;
        sf::Vector2i dimensions;

        vector<SceneLayer*> layers;

        Scene(string filename);
        virtual ~Scene();
        void load();
        void render(sf::RenderWindow* render);
        string play(sf::RenderWindow* window);
        void setHUD(InputBox* hud);
        void setPlayer(Player* player);

    protected:

    private:
};

#endif // SCENE_H
