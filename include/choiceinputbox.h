#ifndef CHOICEINPUTBOX_H
#define CHOICEINPUTBOX_H

#include "inputbox.h"

class ChoiceInputBox: public InputBox
{
    public:
        sf::Vector2i mousePos;
        sf::Vector2i topLeft;
        vector<string> options;
        vector<bool> usable;

        sf::RenderTexture* contentTexture;

        ChoiceInputBox(vector<string> options, vector<bool> usable);
        virtual ~ChoiceInputBox();
        void setMousePos(sf::Vector2i mousePos);
        void render(sf::RenderWindow* window, Netmap_Playable* playable);
        void takeInput(sf::Event event, Netmap_Playable* playable);
        void setFocus(string focus);
        void setSubFocus(int subFocus);
        string getFocus();

    protected:

    private:
};

#endif // CHOICEINPUTBOX_H
