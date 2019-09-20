#include "scenelayer.h"

SceneLayer::SceneLayer(string bkgFilename) {
    cout << bkgFilename << '\n';
    this->bkgFilename = bkgFilename;
    this->bkgTexture = imgLoad(bkgFilename);
    this->bkgSprite = sf::Sprite(bkgTexture);
}

SceneLayer::~SceneLayer() {
    //delete this->bkgTexture;
}

void SceneLayer::render(sf::RenderWindow* window) {
    //cout << "Called SceneLayer::render()\n";
    window->draw(this->bkgSprite);
    // Nothing else for the moment, but fill in as you add more
}
