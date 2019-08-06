#ifndef INPUTBOX_H
#define INPUTBOX_H

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>

#include "toolbox.h"

using namespace std;

class Netmap_Playable;
class DataBattle;
class Program;
class DataBattlePiece;
class InputBox
{
    public:
        bool done;

        InputBox();
        virtual ~InputBox();
        void virtual setMousePos(sf::Vector2i mousePos);
        void virtual render(sf::RenderWindow* window, Netmap_Playable* playable);
        void virtual render(sf::RenderWindow* window, DataBattle* playable);
        void virtual takeInput(sf::Event event, Netmap_Playable* playable);
        void virtual takeInput(sf::Event event, DataBattle* playable);
        void virtual setFocus(string focus);
        void virtual setFocus(DataBattlePiece* focus);
        void virtual setSubFocus(int subFocus);
        string virtual getFocus();
        int virtual getSubFocus();

    protected:

    private:
};

const static sf::Font DEFAULT_FONT = fontLoad("Data\\Fonts\\Terminus.ttf");  // I needed someplace to put this

#endif // INPUTBOX_H
