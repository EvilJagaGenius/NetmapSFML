#include "animation.h"

Animation::Animation() {
    this->totalTime = 0;
}

Animation::~Animation() {
    //dtor
}

void Animation::addFrame(double duration, sf::Rect<int> textureRect) {
    this->increments.push_back(duration);
    this->rects.push_back(textureRect);
    this->totalTime += duration;
}

void Animation::animate(sf::Sprite* sprite, float progress) {
    int currentIndex = 0;
    float currentProgress = 0.0;

    while ((currentProgress + this->increments[currentIndex] < progress) && (currentIndex < this->increments.size())) {
        currentProgress += this->increments[currentIndex];
        currentIndex++;
    }
    sprite->setTextureRect(this->rects[currentIndex]);
    //cout << currentIndex << '\n';
    sf::IntRect textureRect = sprite->getTextureRect();
    //cout << "Rect dimensions: " << textureRect.width << ", " << textureRect.height << '\n';
}
