#include "animation.h"

Animation::Animation() {
    // Don't think we need to do anything here
}

Animation::~Animation() {
    //dtor
}

void Animation::addFrame(float duration, sf::Rect<int> textureRect) {
    this->increments.push_back(duration);
    this->rects.push_back(textureRect);
}

void Animation::animate(sf::Sprite sprite, float progress) {
    int currentIndex = 0;
    float currentProgress = 0.0;

    while ((currentProgress + this->increments[currentIndex] < progress) && (currentIndex < size(this->increments))) {
        currentProgress += this->increments[currentIndex];
        currentIndex++;
    }
    sprite.setTextureRect(this->rects[currentIndex]);
}
