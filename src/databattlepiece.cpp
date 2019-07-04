#include "databattlepiece.h"

DataBattlePiece::DataBattlePiece()
{
    //ctor
}

DataBattlePiece::~DataBattlePiece()
{
    //dtor
}

void DataBattlePiece::load() {}
void DataBattlePiece::move(sf::Vector2i coord, bool firstTime) {}
void DataBattlePiece::addSector(sf::Vector2i coord, int pos) {}
void DataBattlePiece::useAction(DataBattle* level, int actionIndex, sf::Vector2i targetCoord) {}
void DataBattlePiece::switchToAiming(int actionIndex) {}
void DataBattlePiece::noAction() {}
void DataBattlePiece::takeDamage(int damage) {}
void DataBattlePiece::grow(DataBattle* level, int amtToGrow) {}
void DataBattlePiece::prepForTurn() {}
