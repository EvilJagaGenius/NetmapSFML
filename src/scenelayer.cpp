#include "scenelayer.h"

SceneLayer::SceneLayer(string bkgFilename) {
    cout << bkgFilename << '\n';
    this->bkgFilename = bkgFilename;
    this->bkgTexture = imgLoad(bkgFilename);
    this->bkgSprite = sf::Sprite(bkgTexture);
    this->textureDimensions = sf::Vector2<int>(this->bkgTexture.getSize().x, this->bkgTexture.getSize().y);

    this->textureRect = sf::Rect<int>(0, 0, WX, WY);
    this->textureRect.left = ((this->textureDimensions.x / 2) - (WX / 2));
    this->textureRect.top = ((this->textureDimensions.y / 2) - (WY / 2));
    this->bkgSprite.setTextureRect(this->textureRect);
}

SceneLayer::~SceneLayer() {
    //delete this->bkgTexture;
}

void SceneLayer::render(sf::RenderWindow* window) {
    //cout << "Called SceneLayer::render()\n";
    window->draw(this->bkgSprite);
    // Nothing else for the moment, but fill in as you add more
}

void SceneLayer::scroll(sf::Vector2i deltaV) {
    // Do something, Taipu
    this->textureRect.left += (deltaV.x * this->scrollRateX);
    this->bkgSprite.setTextureRect(this->textureRect);
}
