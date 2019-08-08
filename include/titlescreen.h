#ifndef TITLESCREEN_H
#define TITLESCREEN_H

#include <dirent.h>

#include "netmap_playable.h"
#include "textinputbox.h"  // Not sure if we'll need this yet

class TitleScreen: public Netmap_Playable
{
    public:
        sf::Rect<int> button;
        sf::RectangleShape buttonGraphic;
        sf::Text textBox;
        sf::Vector2i mousePos;
        InputBox* currentInputBox;

        TitleScreen();
        virtual ~TitleScreen();
        void setHUD(InputBox* hud);
        void setPlayer(Player* player);
        void render(sf::RenderWindow* window);
        string play(sf::RenderWindow* window);

    protected:

    private:
};

#endif // TITLESCREEN_H
