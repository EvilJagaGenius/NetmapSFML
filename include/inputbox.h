#ifndef INPUTBOX_H
#define INPUTBOX_H

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>

using namespace std;

class Netmap_Playable;
class DataBattle;
class InputBox
{
    public:
        InputBox();
        virtual ~InputBox();
        void virtual setMousePos(sf::Vector2i mousePos);
        void virtual render(sf::RenderWindow* window);
        void virtual takeInput(sf::Event event, Netmap_Playable* playable);
        void virtual takeInput(sf::Event event, DataBattle* playable);
        void virtual setFocus(string focus);
        string virtual getFocus();

    protected:

    private:
};

#endif // INPUTBOX_H
