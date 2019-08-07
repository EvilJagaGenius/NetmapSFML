#ifndef SECTORINPUTBOX_H
#define SECTORINPUTBOX_H

#include "inputbox.h"

class SectorInputBox: public InputBox
{
    public:
        sf::Vector2i mousePos;
        sf::Vector2i coord;
        sf::RectangleShape boxGraphic;
        sf::Text textBox;

        string focus;

        SectorInputBox();
        virtual ~SectorInputBox();
        void render(sf::RenderWindow* window, Netmap_Playable* playable);
        void takeInput(sf::Event event, Netmap_Playable* playable);
        void setMousePos(sf::Vector2i mousePos);
        int getSubFocus();
        string getFocus();

    protected:

    private:
};

#endif // SECTORINPUTBOX_H
