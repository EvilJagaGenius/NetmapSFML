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
    for (SceneEntity* e : this->entities) {
        delete e;
    }
}

void SceneLayer::render(sf::RenderWindow* window) {
    //cout << "Called SceneLayer::render()\n";
    window->draw(this->bkgSprite);

    for (SceneEntity* e : this->entities) {
        if (!e->dummyGraphics) {
            window->draw(e->sprite);
        }
    }
}

void SceneLayer::scroll(sf::Vector2i deltaV) {
    // Do something, Taipu
    this->textureRect.left += (deltaV.x * this->scrollRateX);
    this->textureRect.top += (deltaV.y * this->scrollRateY);
    this->bkgSprite.setTextureRect(this->textureRect);

    for (SceneEntity* e : this->entities) {
        e->sprite.setPosition(e->coord.x - this->textureRect.left, e->coord.y - this->textureRect.top);
    }
}

void SceneLayer::addEntity(SceneEntity* newEntity) {
    this->entities.push_back(newEntity);
    newEntity->sprite.setPosition(newEntity->coord.x - this->textureRect.left, newEntity->coord.y - this->textureRect.top);
}

void SceneLayer::frameTick() {
    for (SceneEntity* e : this->entities) {
        e->frameTick();
    }
}
