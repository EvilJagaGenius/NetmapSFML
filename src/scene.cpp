#include "scene.h"

Scene::Scene(string filename) {
    this->filename = filename;
    this->playableType = 's';

    this->load();
}

Scene::~Scene() {
    //dtor
}

void Scene::load() {
    // Do something, Taipu
    // You wrote a system for doing this in Python.  Copy it over.
    // Actually, on second thought, the Python system... can be improved.  I think we can do without the Layer class we had before.
}

void Scene::setHUD(InputBox* hud) {
    this->hud = hud;
}

void Scene::setPlayer(Player* player) {
    this->player = player;
}

void Scene::render(sf::RenderWindow* window) {
    // Do something, Taipu
}

string Scene::play(sf::RenderWindow* window) {
    bool clicked;

    // Main loop
    while (window->isOpen()) {
        window->clear();
        this->mousePos = sf::Mouse::getPosition(*window);
        clicked = false;
    }

    return "quit:";
}
