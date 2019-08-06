#ifndef CHOICEINPUTBOX_H
#define CHOICEINPUTBOX_H

#include "inputbox.h"

class ChoiceInputBox: public InputBox
{
    public:
        sf::Vector2i mousePos;
        sf::Vector2f topLeft;
        vector<string> options;
        vector<bool> usable;
        int optionIndex;
        int maxSize;
        int subFocus;

        sf::Rect<int> buttonRect;
        sf::RectangleShape buttonGraphic;
        sf::RectangleShape boxGraphic;
        sf::Text textBox;

        ChoiceInputBox(sf::Vector2i topLeft, vector<string> options, vector<bool> usable, int maxSize);
        virtual ~ChoiceInputBox();
        void setMousePos(sf::Vector2i mousePos);
        void render(sf::RenderWindow* window, Netmap_Playable* playable);
        void takeInput(sf::Event event, Netmap_Playable* playable);
        void setFocus(string focus);
        void setSubFocus(int subFocus);
        string getFocus();
        int getSubFocus();

    protected:

    private:
};

#endif // CHOICEINPUTBOX_H
