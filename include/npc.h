#ifndef NPC_H
#define NPC_H

#include <SFML/Graphics.hpp>

#include "netmap_playable.h"

// An NPC is a box to hold a conversation

using namespace std;

class NPC: public Netmap_Playable
{
    public:
        string filename;
        sf::Texture bkgTexture;
        sf::Sprite bkgSprite;

        NPC(string filename);
        virtual ~NPC();
        void load();
        void render(sf::RenderWindow* window);
        string play(sf::RenderWindow* window);

    protected:

    private:
};

#endif // NPC_H
