#include "programaction.h"

// ProgramAction
ProgramAction::ProgramAction() {}
ProgramAction::~ProgramAction() {}
void ProgramAction::use(Netmap_Playable* db, DataBattlePiece* source, sf::Vector2i targetCoord) {}
vector<sf::Vector2i> ProgramAction::getAimArea(sf::Vector2i origin) {
    return getRadius(this->range, origin, false);
}
bool ProgramAction::checkPrereqs(DataBattlePiece* p) {
    return true;
}

// Slice (use as template)
Slice::Slice() {
    this->actionName = "Slice";
    this->description = "Slice \n Delete 2 sectors from target";
    this->range = 1;
    this->targetSprite = 0;
}
Slice::~Slice() {}
void Slice::use(Netmap_Playable* db, DataBattlePiece* source, sf::Vector2i targetCoord) {
    sf::Vector2i origin = source->sectors[0].coord;
    DataBattlePiece* target;

    // Search defenders
    if ((abs(targetCoord.x - origin.x) + abs(targetCoord.y - origin.y) <= this->range) && (targetCoord != origin)) {  // If in range
        for (pair<string, DataBattlePiece*> p : db->defenders) {
            for (ProgramSector s : p.second->sectors) {
                if (s.coord == targetCoord) {
                    // We've found our target
                    target = p.second;
                    break;
                }
            }
            if (target != nullptr) {
                break;
            }
        }
    }

    // Search friendlies (delete if you want to disable friendly fire)
    if (target == nullptr) {
        for (DataBattlePiece* p : db->friendlies) {
            for (ProgramSector s : p->sectors) {
                if (s.coord == targetCoord) {
                    target = p;
                    break;
                }
            }
            if (target != nullptr) {
                break;
            }
        }
    }

    // Take damage
    if (target != nullptr) {
        target->takeDamage(2);
    }
}

// Stone
Stone::Stone() {
    this->actionName = "Stone";
    this->description = "Stone \n Range: 3 \n Delete 1 sector from target";
    this->range = 3;
    this->targetSprite = 0;
}
Stone::~Stone() {}
void Stone::use(Netmap_Playable* db, DataBattlePiece* source, sf::Vector2i targetCoord) {
    sf::Vector2i origin = source->sectors[0].coord;
    DataBattlePiece* target;

    // Search defenders
    if ((abs(targetCoord.x - origin.x) + abs(targetCoord.y - origin.y) <= this->range) && (targetCoord != origin)) {  // If in range
        for (pair<string, DataBattlePiece*> p : db->defenders) {
            for (ProgramSector s : p.second->sectors) {
                if (s.coord == targetCoord) {
                    // We've found our target
                    target = p.second;
                    break;
                }
            }
            if (target != nullptr) {
                break;
            }
        }
    }

    // Search friendlies (delete if you want to disable friendly fire)
    if (target == nullptr) {
        for (DataBattlePiece* p : db->friendlies) {
            for (ProgramSector s : p->sectors) {
                if (s.coord == targetCoord) {
                    target = p;
                    break;
                }
            }
            if (target != nullptr) {
                break;
            }
        }
    }

    // Take damage
    if (target != nullptr) {
        target->takeDamage(1);
    }
}

// Cut
Cut::Cut() {
    this->actionName = "Cut";
    this->description = "Cut \n Delete 2 sectors from target";
    this->range = 1;
    this->targetSprite = 0;
}
Cut::~Cut() {}
void Cut::use(Netmap_Playable* db, DataBattlePiece* source, sf::Vector2i targetCoord) {
    sf::Vector2i origin = source->sectors[0].coord;
    DataBattlePiece* target;

    // Search friendlies
    for (DataBattlePiece* p : db->friendlies) {
        for (ProgramSector s : p->sectors) {
            if (s.coord == targetCoord) {
                target = p;
                break;
            }
        }
        if (target != nullptr) {
            break;
        }
    }

    // Take damage
    if (target != nullptr) {
        target->takeDamage(2);
    }
}
