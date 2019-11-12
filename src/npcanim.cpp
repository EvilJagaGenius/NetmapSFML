#include "npcanim.h"

NPCAnim::NPCAnim() {
    this->visible = true;
}

NPCAnim::NPCAnim(string sheetFilename) {
    this->visible = true;
    this->sheetFilename = sheetFilename;

    this->load();
}

NPCAnim::~NPCAnim(){
    for (pair<string, Animation*> p : this->animations) {
        delete p.second;
    }
}

void NPCAnim::load() {
    this->spritesheet = imgLoad(this->sheetFilename);
    this->sprite = sf::Sprite(this->spritesheet);
    this->sprite.setPosition(100, 100);
    this->loop = true;  // Just by default
}

void NPCAnim::changeAnimation(string newAnimation) {
    this->currentAnimation = this->animations[newAnimation];
}

void NPCAnim::tick() {
    this->progress += (1.0/60.0);  // We tick every frame, 1/60th of a second
    if (this->loop) {
        while (this->progress > this->currentAnimation->totalTime) {
            this->progress -= this->currentAnimation->totalTime;
        }
    }
    this->currentAnimation->animate(&this->sprite, this->progress);
    sf::IntRect textureRect = this->sprite.getTextureRect();
    //cout << "Rect dimensions: " << textureRect.width << ", " << textureRect.height << '\n';
}

void NPCAnim::addAnimation(string animName, Animation* newAnimation) {
    this->animations.insert({{animName, newAnimation}});
    this->currentAnimation = newAnimation;
}
