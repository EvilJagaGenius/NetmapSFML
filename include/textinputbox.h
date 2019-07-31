#ifndef TEXTINPUTBOX_H
#define TEXTINPUTBOX_H

#include "inputbox.h"

class TextInputBox: public InputBox
{
    public:
        bool done;
        sf::Vector2i mousePos;
        sf::Vector2i topLeft;
        sf::Rect<int> rect;
        string prompt;
        string input;

        sf::RectangleShape boxGraphic;
        sf::Text textBox;

        TextInputBox();
        virtual ~TextInputBox();
        void setMousePos(sf::Vector2i mousePos);
        void render(sf::RenderWindow* window, Netmap_Playable* playable);
        void takeInput(sf::Event event, Netmap_Playable* playable);
        void setFocus(string focus);
        void setSubFocus(int subFocus);
        string virtual getFocus();

    protected:

    private:
};

#endif // TEXTINPUTBOX_H
