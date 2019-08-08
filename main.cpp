#include <iostream>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "toolbox.h"
#include "programaction.h"
#include "databattle.h"
#include "databattleeditor.h"
#include "hud.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1024, 576), "NETMAP 1.0");
    window.setFramerateLimit(60);

    HUD* testHUD = new HUD();
    Player* PLAYER = new Player;
    testHUD->setPlayer(PLAYER);

    Netmap_Playable* CURRENT_PLAYABLE = new DataBattleEditor("");
    string nextPlayable;
    //Netmap_Playable* CURRENT_PLAYABLE = new DataBattle("TestBattle");
    while (true) {
        CURRENT_PLAYABLE->setHUD(testHUD);
        CURRENT_PLAYABLE->setPlayer(PLAYER);
        nextPlayable = CURRENT_PLAYABLE->play(&window);
        delete CURRENT_PLAYABLE;
        if (startsWith(nextPlayable, "quit:")) {
            break;
        } else if (startsWith(nextPlayable, "editor:")) {
            CURRENT_PLAYABLE = new DataBattleEditor(splitString(nextPlayable, ':')[1]);
        } else if (startsWith(nextPlayable, "db:")) {
            CURRENT_PLAYABLE = new DataBattle(splitString(nextPlayable, ':')[1]);
        } else if (startsWith(nextPlayable, "dbFromEditor:")) {
            CURRENT_PLAYABLE = new DataBattle(splitString(nextPlayable, ':')[1]);
            CURRENT_PLAYABLE->destination = "editor:" + splitString(nextPlayable, ':')[1];
        } else if (startsWith(nextPlayable, "title:")) {
            // Do something, Taipu
            break;
        }
    }

    return 0;
}
