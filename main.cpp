#include <iostream>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

#include "databattle.h"
#include "toolbox.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1024, 576), "NETMAP 1.0");

    // Databattle testing
    HUD* testHUD = new HUD();
    DataBattle* testBattle = new DataBattle("TestBattle");
    testBattle->play(&window, testHUD);

    return 0;
}
