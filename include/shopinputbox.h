#ifndef SHOPINPUTBOX_H
#define SHOPINPUTBOX_H

#include "inputbox.h"
#include "program.h"
#include "player.h"
#include "netmap_playable.h"

class ShopInputBox: public InputBox
{
    public:
        sf::Vector2i mousePos;
        sf::Vector2i topLeft;
        sf::Rect<int> rect;
        sf::Rect<int> buttonRect;
        sf::RectangleShape boxGraphic;
        sf::RectangleShape buttonGraphic;
        sf::Text textBox;
        vector<Program*> programList;
        string focus;
        int highlightedBox;

        ShopInputBox();
        virtual ~ShopInputBox();
        void setMousePos(sf::Vector2i mousePos);
        void render(sf::RenderWindow* window, Netmap_Playable* playable);
        void takeInput(sf::Event event, Netmap_Playable* playable);
        string getFocus();

    protected:

    private:
};

#endif // SHOPINPUTBOX_H
