#ifndef HUD_H
#define HUD_H

#include <iostream>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

#include "databattle.h"
#include "toolbox.h"
#include "player.h"
#include "program.h"

class HUD: public InputBox
{
    public:
        // Variables
        bool open;
        sf::Vector2i mousePos;

        sf::Texture panelTexture;
        sf::Sprite panelSprite;
        sf::Texture borderTexture;
        sf::Sprite borderSprite;
        sf::IntRect borderRect;

        sf::RenderTexture contentTexture;
        sf::Sprite contentSprite;

        sf::Sprite focusSprite;
        char focusType;
        Program* focusProgram;
        sf::Vector2i focusCoord;
        sf::Text focusTextBox;
        int subFocus;

        Player* player;
        int programListIndex;

        // Functions
        HUD();
        virtual ~HUD();
        void setMousePos(sf::Vector2i mousePos);
        void takeInput(sf::Event event, Netmap_Playable* playable);
        void takeInput(sf::Event event, DataBattle* playable);
        void render(sf::RenderWindow* window, DataBattle* playable);
        void setPlayer(Player* p);
        void setFocus(Program* focus);
        void setSubFocus(int subFocus);

    protected:

    private:
};

#endif // HUD_H
