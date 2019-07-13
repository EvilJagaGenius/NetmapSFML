#include "programaction.h"

// ProgramAction
ProgramAction::ProgramAction() {
    this->numOfTargets = 1;
}
ProgramAction::~ProgramAction() {}
void ProgramAction::use(Netmap_Playable* db, DataBattlePiece* source, sf::Vector2i targetCoord) {
    cout << "Called ProgramAction::use()\n";
}
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
    //cout << "Using Slice\n";
    sf::Vector2i origin = source->sectors[0]->coord;
    DataBattlePiece* target = nullptr;

    // Search defenders
    if ((abs(targetCoord.x - origin.x) + abs(targetCoord.y - origin.y) <= this->range) && (targetCoord != origin)) {  // If in range
        for (pair<string, DataBattlePiece*> p : db->defenders) {
            for (ProgramSector* s : p.second->sectors) {
                if (s->coord == targetCoord) {
                    // We've found our target
                    target = p.second;
                    break;
                }
            }
            if (target != nullptr) {
                break;
            }
        }

        // Search friendlies (delete if you want to disable friendly fire)
        if (target == nullptr) {
            for (DataBattlePiece* p : db->friendlies) {
                for (ProgramSector* s : p->sectors) {
                    if (s->coord == targetCoord) {
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
}

Slash::Slash() {
    this->actionName = "Slash";
    this->description = "Slash \n Size required: max \n Delete 3 sectors from target";
    this->range = 1;
    this->targetSprite = 0;
}
Slash::~Slash() {}
void Slash::use(Netmap_Playable* db, DataBattlePiece* source, sf::Vector2i targetCoord) {
    //cout << "Using Slash\n";
    if (checkPrereqs(source)) {
        sf::Vector2i origin = source->sectors[0]->coord;
        DataBattlePiece* target = nullptr;

        // Search defenders
        if ((abs(targetCoord.x - origin.x) + abs(targetCoord.y - origin.y) <= this->range) && (targetCoord != origin)) {  // If in range
            for (pair<string, DataBattlePiece*> p : db->defenders) {
                for (ProgramSector* s : p.second->sectors) {
                    if (s->coord == targetCoord) {
                        // We've found our target
                        target = p.second;
                        break;
                    }
                }
                if (target != nullptr) {
                    break;
                }
            }

            // Search friendlies (delete if you want to disable friendly fire)
            if (target == nullptr) {
                for (DataBattlePiece* p : db->friendlies) {
                    for (ProgramSector* s : p->sectors) {
                        if (s->coord == targetCoord) {
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
            target->takeDamage(3);
            }
        }
    }
}
bool Slash::checkPrereqs(DataBattlePiece* p) {
    if (p->size >= p->maxSize) {
        return true;
    }
    return false;
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
    //cout << "Using Stone\n";
    sf::Vector2i origin = source->sectors[0]->coord;
    DataBattlePiece* target = nullptr;

    // Search defenders
    if ((abs(targetCoord.x - origin.x) + abs(targetCoord.y - origin.y) <= this->range) && (targetCoord != origin)) {  // If in range
        for (pair<string, DataBattlePiece*> p : db->defenders) {
            for (ProgramSector* s : p.second->sectors) {
                if (s->coord == targetCoord) {
                    // We've found our target
                    target = p.second;
                    break;
                }
            }
            if (target != nullptr) {
                break;
            }
        }
        // Search friendlies (delete if you want to disable friendly fire)
        if (target == nullptr) {
            for (DataBattlePiece* p : db->friendlies) {
                for (ProgramSector* s : p->sectors) {
                    if (s->coord == targetCoord) {
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
}

Sling1::Sling1() {
    this->actionName = "Sling1";
    this->description = "Sling1 \n Range: 2 \n Warp program to selected grid sector";
    this->range = 2;
    this->targetSprite = 1;
}
Sling1::~Sling1() {}
void Sling1::use(Netmap_Playable* db, DataBattlePiece* source, sf::Vector2i targetCoord) {
    //cout << "Using Sling1\n";
    sf::Vector2i origin = source->sectors[0]->coord;
    if (abs(targetCoord.x - origin.x) + abs(targetCoord.y - origin.y) <= this->range && (targetCoord != origin)) {  // If in range
        // Calculate the vector we're going to warp along.
        sf::Vector2i warpVector = sf::Vector2<int>(targetCoord.x - origin.x, targetCoord.y - origin.y);
        vector<sf::Vector2i> coordsToAmputate;
        // Shift every sector in the program by that vector.
        for (ProgramSector* s : source->sectors) {
            sf::Vector2i newCoord = sf::Vector2<int>(s->coord.x + warpVector.x, s->coord.y + warpVector.y);
            if (!startsWith(db->lookAt(newCoord), "tile")) {  // If that coord didn't land on a tile
                coordsToAmputate.push_back(newCoord);  // Mark it for amputation
            }
            s->coord = newCoord;
        }
        // Loop through all the sectors we marked and amputate them
        for (sf::Vector2i coord : coordsToAmputate) {
            source->amputate(coord);
        }
    }
}

Grow::Grow() {
    this->actionName = "Grow";
    this->description = "Grow \n Add 2 sectors to target";
    this->range = 1;
    this->targetSprite = 2;
}
Grow::~Grow() {}
void Grow::use(Netmap_Playable* db, DataBattlePiece* source, sf::Vector2i targetCoord) {
    //cout << "Using Grow\n";
    sf::Vector2i origin = source->sectors[0]->coord;
    DataBattlePiece* target = nullptr;

    if (abs(targetCoord.x - origin.x) + abs(targetCoord.y - origin.y) <= this->range && (targetCoord != origin)) {  // If in range
        for (pair<string, DataBattlePiece*> p : db->defenders) {
            for (ProgramSector* s : p.second->sectors) {
                if (s->coord == targetCoord) {
                    // We've found our target
                    target = p.second;
                    break;
                }
            }
            if (target != nullptr) {
                break;
            }
        }
        // Search friendlies (delete if you want to disable friendly fire)
        if (target == nullptr) {
            for (DataBattlePiece* p : db->friendlies) {
                for (ProgramSector* s : p->sectors) {
                    if (s->coord == targetCoord) {
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
            target->grow(db, 2);
        }
    }
}

Surgery1::Surgery1() {
    this->actionName = "Surgery1";
    this->description = "Surgery1 \n Size req: max \n Add 1 to target's maximum size";
    this->range = 1;
    this->targetSprite = 2;
}
Surgery1::~Surgery1() {}
void Surgery1::use(Netmap_Playable* db, DataBattlePiece* source, sf::Vector2i targetCoord) {
    //cout << "Using Surgery1\n";
    sf::Vector2i origin = source->sectors[0]->coord;
    DataBattlePiece* target = nullptr;

    if (this->checkPrereqs(source)) {
        if (abs(targetCoord.x - origin.x) + abs(targetCoord.y - origin.y) <= this->range && (targetCoord != origin)) {  // If in range
            for (pair<string, DataBattlePiece*> p : db->defenders) {
                for (ProgramSector* s : p.second->sectors) {
                    if (s->coord == targetCoord) {
                        // We've found our target
                        target = p.second;
                        break;
                    }
                }
                if (target != nullptr) {
                    break;
                }
            }
            // Search friendlies (delete if you want to disable friendly fire)
            if (target == nullptr) {
                for (DataBattlePiece* p : db->friendlies) {
                    for (ProgramSector* s : p->sectors) {
                        if (s->coord == targetCoord) {
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
                // You should replace this with a general-purpose function that we can override for non-programs.  Say, users and avatars
                target->maxSize++;
            }
        }
    }
}
bool Surgery1::checkPrereqs(DataBattlePiece* p) {
    if (p->size >= p->maxSize) {
        return true;
    }
    return false;
}

Glitch::Glitch() {
    this->actionName = "Glitch";
    this->description = "Glitch \n Delete 2 sectors from target";
    this->range = 1;
    this->targetSprite = 0;
}
Glitch::~Glitch() {}
void Glitch::use(Netmap_Playable* db, DataBattlePiece* source, sf::Vector2i targetCoord) {
    sf::Vector2i origin = source->sectors[0]->coord;
    DataBattlePiece* target = nullptr;

    if (abs(targetCoord.x - origin.x) + abs(targetCoord.y - origin.y) <= this->range && (targetCoord != origin)) {  // If in range
        for (pair<string, DataBattlePiece*> p : db->defenders) {
            for (ProgramSector* s : p.second->sectors) {
                if (s->coord == targetCoord) {
                    // We've found our target
                    target = p.second;
                    break;
                }
            }
            if (target != nullptr) {
                break;
            }
        }
        // Search friendlies (delete if you want to disable friendly fire)
        if (target == nullptr) {
            for (DataBattlePiece* p : db->friendlies) {
                for (ProgramSector* s : p->sectors) {
                    if (s->coord == targetCoord) {
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
    //cout << "Using Cut\n";
    sf::Vector2i origin = source->sectors[0]->coord;
    DataBattlePiece* target;

    if (abs(targetCoord.x - origin.x) + abs(targetCoord.y - origin.y) <= this->range) {  // If in range
        // Search friendlies
        for (DataBattlePiece* p : db->friendlies) {
            for (ProgramSector* s : p->sectors) {
                if (s->coord == targetCoord) {
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
}
