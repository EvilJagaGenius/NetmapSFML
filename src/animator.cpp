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

}

void Animator::update(sf::Time dt) {
    // Do something similar to the float version of this
}

void Animator::update(float progress) {
    this->progress += progress;
    if (loop) {
        while (this->progress >= this->loopTime) {
            this->progress -= this->loopTime;
        }
    }
}

void Animator::animate(sf::Sprite* sprite, float progress) {
    currentAnim.animate(sprite, progress);
}
