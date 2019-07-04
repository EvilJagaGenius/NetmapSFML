#include <iostream>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

#include "toolbox.h"
#include "programaction.h"
#include "databattle.h"
#include "hud.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1024, 576), "NETMAP 1.0");

    // Databattle testing
    HUD* testHUD = new HUD();
    Player* PLAYER = new Player;
    testHUD->setPlayer(PLAYER);

    Netmap_Playable* CURRENT_PLAYABLE = new DataBattle("TestBattle");
    CURRENT_PLAYABLE->setHUD(testHUD);
    CURRENT_PLAYABLE->setPlayer(PLAYER);
    CURRENT_PLAYABLE->play(&window);

    /*
    Netmap_Playable* testBattle = new DataBattle("TestBattle");
    testBattle->play(&window, testHUD);
    */

    return 0;
}
