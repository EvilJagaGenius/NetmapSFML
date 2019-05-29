#include <iostream>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include <databattle.h>


sf::Texture imgLoad(std::string filename) {
    sf::Texture texture;
    texture.loadFromFile(filename);
    return texture;
}

int main()
{
    // Default SFML test program

    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    // Databattle testing
    DataBattle* testBattle = new DataBattle("testBattle");

    return 0;
}
