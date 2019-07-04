#ifndef INPUTBOX_H
#define INPUTBOX_H

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>

using namespace std;

class Netmap_Playable;
class DataBattle;
class Program;
class InputBox
{
    public:

        InputBox();
        virtual ~InputBox();
        void virtual setMousePos(sf::Vector2i mousePos);
        void virtual render(sf::RenderWindow* window, Netmap_Playable* playable);
        void virtual render(sf::RenderWindow* window, DataBattle* playable);
        void virtual takeInput(sf::Event event, Netmap_Playable* playable);
        void virtual takeInput(sf::Event event, DataBattle* playable);
        void virtual setFocus(string focus);
        void virtual setFocus(Program* focus);
        void virtual setSubFocus(int subFocus);
        string virtual getFocus();

    protected:

    private:
};

#endif // INPUTBOX_H
