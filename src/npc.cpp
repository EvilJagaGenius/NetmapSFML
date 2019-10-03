#include "npc.h"

NPC::NPC(string filename) {
    this->filename = filename;
}

NPC::~NPC() {}  // Don't think we need this yet

void NPC::load() {

}

void NPC::render(sf::RenderWindow* window) {
    // Do something, Taipu
}

string NPC::play(sf::RenderWindow* window) {
    while (window->isOpen()) {
        window->clear();

        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window->close();
            }
        }

        this->render(window);
        window->display();
    }

    return "quit:";
}
