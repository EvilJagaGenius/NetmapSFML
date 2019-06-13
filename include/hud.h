#ifndef HUD_H
#define HUD_H

#include <iostream>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

#include "toolbox.h"

const static sf::Font DEFAULT_FONT = fontLoad("Data\\Fonts\\Terminus.ttf");  // I needed someplace to put this
const static int WX = 1024;
const static int WY = 576;

class DataBattle;
class HUD
{
    public:
        // Variables
        bool open;
        sf::Vector2i mousePos;

        sf::Sprite focusSprite;
        sf::Texture panelTexture;
        sf::Sprite panelSprite;
        sf::Texture borderTexture;
        sf::Sprite borderSprite;
        sf::IntRect borderRect;

        sf::RenderTexture contentTexture;
        sf::Sprite contentSprite;

        // Functions
        HUD();
        virtual ~HUD();
        void setMousePos(sf::Vector2i mousePos);
        void takeInput(sf::Event event);
        void render(sf::RenderWindow* window);

    protected:

    private:
};

#endif // HUD_H
