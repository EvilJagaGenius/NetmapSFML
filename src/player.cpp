#include "player.h"

Player::Player()
{
    //ctor

    this->giveStartingPrograms();
}

Player::~Player()
{
    //dtor
}

void Player::giveStartingPrograms() {
    // Debug mode
    this->programs.insert({{"Hack", 10},
                          {"Slingshot", 10},
                          {"DataDr", 10},
                          {"BitMan", 10},
                          {"Bug", 10},
                          {"Watchman", 10}});
}

void Player::readyup() {
    this->ready = true;
}
