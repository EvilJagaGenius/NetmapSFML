#include "databattlepiece.h"

DataBattlePiece::DataBattlePiece()
{
    //ctor
}

DataBattlePiece::~DataBattlePiece()
{
    //dtor
}
void DataBattlePiece::deleteSectors() {}
void DataBattlePiece::load() {}
void DataBattlePiece::move(sf::Vector2i coord, bool firstTime) {}
void DataBattlePiece::addSector(sf::Vector2i coord, int pos) {}
void DataBattlePiece::useAction(Netmap_Playable* level, int actionIndex, vector<sf::Vector2i> targets) {}
void DataBattlePiece::switchToAiming(int actionIndex) {}
void DataBattlePiece::noAction() {}
void DataBattlePiece::takeDamage(int damage) {}
void DataBattlePiece::amputate(sf::Vector2i coord) {}
void DataBattlePiece::grow(Netmap_Playable* level, int amtToGrow) {
    cout << "Called DataBattlePiece::grow()\n";
}
void DataBattlePiece::prepForTurn() {}