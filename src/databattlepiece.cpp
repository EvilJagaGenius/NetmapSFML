#include "databattlepiece.h"

DataBattlePiece::DataBattlePiece()
{
    this->statuses =   {{'g', 0},  // Gridwalk.  Move anywhere on the grid regardless of it being a tile or not
                        {'b', 0},  // Gridburn.  Each sector deleted from a tile sets that grid sector to empty
                        {'n', 0},  // Negawalk.  Only move on blank grid sectors
                        {'i', 0},  // Invisibility.
                        {'a', 0},  // Alarm.  Attacks made on you deal an extra point of damage
                        {'t', 0},  // Traced.  Can't turn invisible and any mines you leave will be visible
                        {'w', 0},  // Webbed.  -1 speed debuff
                        {'f', 0},  // Frozen.  Speed locked to 0
                        {'s', 0},  // Shutdown.  Lose a turn
                        {'l', 0},  // Locked.  Can't warp or teleport
                        {'c', 0},  // Capped.  Can't grow/heal
                        {'p', 0},  // Poison.  Take a point of damage on finishing the turn
                        {'h', 0}}; // Hacked.  Another player has taken control of the piece
}

DataBattlePiece::~DataBattlePiece()
{
    //dtor
}
void DataBattlePiece::deleteSectors() {}
void DataBattlePiece::load() {}
void DataBattlePiece::move(Netmap_Playable* level, sf::Vector2i coord, bool firstTime) {}
void DataBattlePiece::addSector(sf::Vector2i coord, int pos) {}
void DataBattlePiece::useAction(Netmap_Playable* level, int actionIndex, vector<sf::Vector2i> targets) {}
void DataBattlePiece::switchToAiming(int actionIndex) {}
void DataBattlePiece::noAction() {}
void DataBattlePiece::takeDamage(Netmap_Playable* level, int damage) {}
void DataBattlePiece::amputate(Netmap_Playable* level, sf::Vector2i coord) {}
void DataBattlePiece::grow(Netmap_Playable* level, int amtToGrow) {
    cout << "Called DataBattlePiece::grow()\n";
}
void DataBattlePiece::prepForTurn() {}
