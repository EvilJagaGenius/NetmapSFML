#include "animator.h"

Animator::Animator()
{
    //ctor
}

Animator::~Animator()
{
    //dtor
}

void Animator::addAnimation(string id, Animation anim) {
    this->animations.insert({{id, anim}});
}

void Animator::playAnimation(string id, bool loop) {
    this->loop = loop;
    this->progress = 0;
    this->currentAnim = this->animations[id];
    this->loopTime = this->currentAnim.totalTime;
}

void Animator::update(sf::Time dt) {
    // Do something similar to the float version of this
}

void Animator::update(float progress) {
    this->progress += progress;
    if (this->loop) {
        while (this->progress >= this->loopTime) {
            this->progress -= this->loopTime;
        }
    }
}

void Animator::animate(sf::Sprite* target, float progress) {
    currentAnim.animate(target, progress);
}

void Animator::animate(sf::Sprite* target) {
    currentAnim.animate(target, this->progress);
}
