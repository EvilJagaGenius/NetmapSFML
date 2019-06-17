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
    this->programs.insert({{"Hack", 1},
                          {"Slingshot", 1}});
}
