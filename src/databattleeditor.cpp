#include "databattleeditor.h"

DataBattleEditor::DataBattleEditor(string dummy) {
    this->db = nullptr;
    this->gridSprite = sf::Sprite(GRID_SHEET);
    this->programSprite = sf::Sprite(PROGRAM_SHEET);
    this->textBox = sf::Text("", DEFAULT_FONT, 12);
}

DataBattleEditor::~DataBattleEditor() {
    if (this->db != nullptr) {
        delete this->db;
    }
}

void DataBattleEditor::newDB(string filename) {
    // Create the file, fill it in
    ofstream newFile;
    newFile.open("Data\\DataBattles\\" + filename);
    // Start writing
    newFile << "bkg:DummyBkg.png\n";
    newFile << "cash:0\n";
    newFile << "music:DummyTrack.ogg\n";
    newFile << "start_grid\n";
    for (int y=0; y<16; y++) {
        for (int x=0; x<16; x++) {
            newFile << "0";
        }
        newFile << "\n";
    }
    newFile << "end_grid\n";
    newFile << "start_commands\n";
    newFile << "end_commands\n";

    // Once that's done:
    this->db = new DataBattle(filename);
}

void DataBattleEditor::loadDB(string filename) {
    if (this->db != nullptr) {
        delete this->db;
    }
    this->db = new DataBattle(filename);
}

void DataBattleEditor::saveDB(string filename) {
    if (this->db != nullptr) {
        // Create the file.  I'm hoping this overwrites it, like in Python
        ofstream newFile;
        newFile.open("Data\\DataBattles\\" + filename);
        // Start writing
        newFile << "bkg:" << this->db->bkgFilename << '\n';
        newFile << "cash:" << to_string(this->db->cashReward) << '\n';
        newFile << "music:" << this->db->musicFilename << '\n';
        newFile << "start_grid\n";
        for (int y=0; y<16; y++) {
            for (int x=0; x<16; x++) {
                newFile << to_string(this->db->grid[x][y]);
            }
            newFile << "\n";
        }
        newFile << "end_grid\n";
        newFile << "start_commands\n";
        for (sf::Vector2i coord : this->db->uploads) {
            newFile << "addUpload:" << to_string(coord.x) << ':' << to_string(coord.y) << '\n';
        }
        for (pair<string, DataBattlePiece*> element : this->db->defenders) {
            DataBattlePiece* p = element.second;
            sf::Vector2i programHead = p->sectors[0]->coord;
            newFile << "addDefender:" << p->programType << ':' <<  to_string(programHead.x) << ':' << to_string(programHead.y) << ':' << element.first << '\n';
            // Add sectors
            for (int i=1; i<p->sectors.size(); i++) {
                sf::Vector2i coord = p->sectors[i]->coord;
                newFile << "addSector:" << element.first << ':' << to_string(coord.x) << ':' << to_string(coord.y) << ':';
                // Find the sector it's connected to (Do something, Taipu)
                bool foundConnection = false;
                for (int j=0; j<p->sectors.size(); j++) {
                    for (ProgramSector* l : p->sectors[i]->links) {
                        if (l->coord == coord) {
                            foundConnection = true;
                            newFile << to_string(j) << '\n';
                            break;
                        }
                    }
                    if (foundConnection) {
                        break;
                    }
                }
            }
        }
        newFile << "end_commands\n";
    }
}

void DataBattleEditor::render(sf::RenderWindow* window) {
    cout << "Rendering\n";
    // Draw new/load buttons
    this->textBox.setColor(sf::Color::White);
    this->textBox.setPosition(WY, 0);
    this->textBox.setString("New DataBattle");
    window->draw(this->textBox);

    this->textBox.setPosition(WY, 14);
    this->textBox.setString("Load DataBattle");
    window->draw(this->textBox);


    if (this->db != nullptr) {
        // Copy code from DataBattle::render()
        window->draw(this->db->bkgSprite);
        sf::Vector2<int> cursorTile(-1, -1);

        // Draw grid
        for (int x=0; x<16; x++) {
            for (int y=0; y<16; y++) {
                sf::Rect<int> tileRect(x*TILE_SIZE + x*GAP_SIZE, y*TILE_SIZE + y*GAP_SIZE, TILE_SIZE, TILE_SIZE);
                this->gridSprite.setTextureRect(sf::Rect<int>(0, this->db->grid[x][y]*TILE_SIZE, TILE_SIZE, TILE_SIZE));
                this->gridSprite.setPosition(sf::Vector2<float>(x*TILE_SIZE + x*GAP_SIZE, y*TILE_SIZE + y*GAP_SIZE));
                window->draw(this->gridSprite);

                if (tileRect.contains(this->mousePos)) {
                    cursorTile = sf::Vector2<int>(x, y);
                }
            }
        }

        // Draw defenders
        for (pair<string, DataBattlePiece*> element : this->db->defenders) {
            DataBattlePiece* p = element.second;
            if (p->visible) {
                this->programSprite.setColor(sf::Color::White);
            } else {
                this->programSprite.setColor(sf::Color(128, 128, 128, 128));
            }

            // Draw connecting lines
            for (int i=0; i<p->size; i++) {
                ProgramSector* sector = p->sectors[i];
                for (ProgramSector* link : sector->links) {
                    sf::Vector2i nextCoord = link->coord;
                    sf::Vector2i centerA = sf::Vector2<int>(sector->coord.x*TILE_SIZE + sector->coord.x*GAP_SIZE + 13, sector->coord.y*TILE_SIZE + sector->coord.y*GAP_SIZE + 13);
                    sf::Vector2i centerB = sf::Vector2<int>(nextCoord.x*TILE_SIZE + nextCoord.x*GAP_SIZE + 13, nextCoord.y*TILE_SIZE + nextCoord.y*GAP_SIZE + 13);
                    sf::RectangleShape connectingLine(sf::Vector2<float>(abs(centerA.x - centerB.x) + 2, abs(centerA.y - centerB.y) + 2));
                    sf::Vector2f topLeft;
                    if (centerA.x < centerB.x) {
                        topLeft.x = centerA.x;
                    } else {
                        topLeft.x = centerB.x;
                    }
                    if (centerA.y < centerB.y) {
                        topLeft.y = centerA.y;
                    } else {
                        topLeft.y = centerB.y;
                    }
                    connectingLine.setFillColor(p->color);
                    connectingLine.setOutlineColor(p->color);
                    connectingLine.setPosition(topLeft);
                    window->draw(connectingLine);
                }
            }
            for (int i=0; i<(p->size); i++) {
                ProgramSector* sector = p->sectors[i];
                if (i==0) {  // Draw head sprite
                    this->programSprite.setTextureRect(sf::Rect<int>(p->spriteCoord.x*TILE_SIZE, p->spriteCoord.y*TILE_SIZE, TILE_SIZE, TILE_SIZE));
                    this->programSprite.setPosition(sector->coord.x*TILE_SIZE + sector->coord.x*GAP_SIZE, sector->coord.y*TILE_SIZE + sector->coord.y*GAP_SIZE);
                    window->draw(this->programSprite);
                } else {  // Draw tail sprite
                    this->programSprite.setTextureRect(sf::Rect<int>(p->spriteCoord.x*TILE_SIZE, (p->spriteCoord.y + 1)*TILE_SIZE, TILE_SIZE, TILE_SIZE));
                    this->programSprite.setPosition(sector->coord.x*TILE_SIZE + sector->coord.x*GAP_SIZE, sector->coord.y*TILE_SIZE + sector->coord.y*GAP_SIZE);
                    window->draw(this->programSprite);
                }
            }
        }

        // Draw friendlies
        for (int i=0; i<this->db->friendlies.size(); i++) {
            DataBattlePiece* p = this->friendlies[i];
            if (!p->visible) {
                this->programSprite.setColor(sf::Color(128,128,128,128));
            } else {
                this->programSprite.setColor(sf::Color::White);
            }
            for (int j=0; j<(p->size); j++) {
                ProgramSector* sector = p->sectors[j];
                for (ProgramSector* link : sector->links) {
                    sf::Vector2i nextCoord = link->coord;
                    sf::Vector2i centerA = sf::Vector2<int>(sector->coord.x*TILE_SIZE + sector->coord.x*GAP_SIZE + 13, sector->coord.y*TILE_SIZE + sector->coord.y*GAP_SIZE + 13);
                    sf::Vector2i centerB = sf::Vector2<int>(nextCoord.x*TILE_SIZE + nextCoord.x*GAP_SIZE + 13, nextCoord.y*TILE_SIZE + nextCoord.y*GAP_SIZE + 13);
                    sf::RectangleShape connectingLine(sf::Vector2<float>(abs(centerA.x - centerB.x) + 2, abs(centerA.y - centerB.y) + 2));
                    sf::Vector2f topLeft;
                    if (centerA.x < centerB.x) {
                        topLeft.x = centerA.x;
                    } else {
                        topLeft.x = centerB.x;
                    }
                    if (centerA.y < centerB.y) {
                        topLeft.y = centerA.y;
                    } else {
                        topLeft.y = centerB.y;
                    }
                    connectingLine.setFillColor(p->color);
                    connectingLine.setOutlineColor(p->color);
                    connectingLine.setPosition(topLeft);
                    window->draw(connectingLine);
                }
            }
            for (int j=0; j<(p->size); j++) {
                ProgramSector* sector = p->sectors[j];
                if (j==0) {  // Draw head sprite
                    this->programSprite.setTextureRect(sf::Rect<int>(p->spriteCoord.x*TILE_SIZE, p->spriteCoord.y*TILE_SIZE, TILE_SIZE, TILE_SIZE));
                    this->programSprite.setPosition(sf::Vector2<float>(sector->coord.x*TILE_SIZE + sector->coord.x*GAP_SIZE, sector->coord.y*TILE_SIZE + sector->coord.y*GAP_SIZE));
                    window->draw(this->programSprite);
                } else {  // Draw tail sprite
                    this->programSprite.setTextureRect(sf::Rect<int>(p->spriteCoord.x*TILE_SIZE, (p->spriteCoord.y + 1)*TILE_SIZE, TILE_SIZE, TILE_SIZE));
                    this->programSprite.setPosition(sf::Vector2<float>(sector->coord.x*TILE_SIZE + sector->coord.x*GAP_SIZE, sector->coord.y*TILE_SIZE + sector->coord.y*GAP_SIZE));
                    window->draw(this->programSprite);
                }
            }
        }
        for (int i=0; i<db->uploads.size(); i++) {
            sf::Vector2i coord = this->db->uploads[i];
            this->gridSprite.setPosition(coord.x*TILE_SIZE + coord.x*GAP_SIZE, coord.y*TILE_SIZE + coord.y*GAP_SIZE);
            this->gridSprite.setTextureRect(sf::Rect<int>(0, 2*TILE_SIZE, TILE_SIZE, TILE_SIZE));
            window->draw(this->gridSprite);
        }
        if (cursorTile.x != -1) {  // If we found a valid tile to highlight
            this->gridSprite.setTextureRect(sf::Rect<int>(0, 3*TILE_SIZE, TILE_SIZE, TILE_SIZE));
            this->gridSprite.setPosition(sf::Vector2<float>(cursorTile.x*TILE_SIZE + cursorTile.x*GAP_SIZE, cursorTile.y*TILE_SIZE + cursorTile.y*GAP_SIZE));
            window->draw(gridSprite);
        }
    }
}

string DataBattleEditor::play(sf::RenderWindow* window) {
    cout << "Called DataBattleEditor::play()\n";
    bool rightClicked = false;
    bool leftClicked = false;

    // Main loop
    while (window->isOpen()) {
        window->clear();
        this->mousePos = sf::Mouse::getPosition(*window);
        leftClicked = false;
        rightClicked = false;

        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window->close();
            } else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    leftClicked = true;
                } else if (event.mouseButton.button == sf::Mouse::Right) {
                    rightClicked = true;
                }
            }
        }
        this->render(window);
        window->display();
    }
    return "";
}
