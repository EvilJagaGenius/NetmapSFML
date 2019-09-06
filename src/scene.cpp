#include "scene.h"

Scene::Scene(string filename) {
    this->filename = filename;
    this->playableType = 's'

    this->load();
}

Scene::~Scene() {
    //dtor
}

void Scene::load() {
    // Do something, Taipu
    // You wrote a system for doing this in Python.  Copy it over.
}

void setHUD(InputBox* hud) {
    this->hud = hud;
}

void setPlayer(Player* player) {
    this->player = player;
}

void render(sf::RenderWindow* window) {
    // Do something, Taipu
}

string play(sf::RenderWindow* window) {

    // Main loop
    while (window->isOpen()) {
        window->clear();
        this->mousePos = sf::Mouse::getPosition(*window);
        clicked = false;
    }

    return "quit:";
}
