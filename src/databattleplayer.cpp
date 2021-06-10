#include "databattleplayer.h"

DataBattlePlayer::DataBattlePlayer() {
    this->localPlayer = nullptr;
    this->hudPanel.create(WY, WY);

    this->hudText = sf::Text("", DEFAULT_FONT, 12);
    this->hudText.setColor(sf::Color::White);
    //this->hudText.setCharacterSize(14);

    this->hudButton.setFillColor(sf::Color::Transparent);
    this->hudButton.setOutlineColor(sf::Color::White);
    this->hudButton.setOutlineThickness(2);
    this->inputBoxType = '0';
}

DataBattlePlayer::DataBattlePlayer(DataBattle* db) {
    this->localPlayer = nullptr;
    this->hudPanel.create(WY, WY);

    this->hudText = sf::Text("", DEFAULT_FONT, 12);
    this->hudText.setColor(sf::Color::White);
    //this->hudText.setCharacterSize(14);

    this->hudButton.setFillColor(sf::Color::Transparent);
    this->hudButton.setOutlineColor(sf::Color::White);
    this->hudButton.setOutlineThickness(2);
    this->inputBoxType = '0';

    this->setDB(db);
}

DataBattlePlayer::~DataBattlePlayer() {
    if (this->inputBox != nullptr) {
        delete this->inputBox;
    }
}

void DataBattlePlayer::render(sf::RenderWindow* window) {
    // Draw the background
    window->draw(this->bkgSprite);
    //window->clear(sf::Color::White);
    sf::Vector2<int> cursorTile(-1, -1);

    // Draw the grid
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
    //cout << "Grid drawn\n";

    // Draw pieces
    for (int i=0; i<this->db->pieces.size(); i++) {
        DataBattlePiece* piece = this->db->pieces[i];
        sf::Sprite spriteToDraw;
        spriteToDraw = this->gridSprite;
        if (piece->pieceType == 'u') {  // Upload zone
            spriteToDraw = this->gridSprite;
        } else {  // Anything else
            spriteToDraw = this->programSprite;
        }

        // Draw sector sprites
        for (int j=0; j<(piece->size); j++) {
            ProgramSector* sector = piece->sectors[j];
            if (j==0) {  // Draw head sprite
                spriteToDraw.setTextureRect(sf::Rect<int>(TILE_SIZE, piece->spriteCoord.y*TILE_SIZE, TILE_SIZE, TILE_SIZE));
                spriteToDraw.setPosition(sf::Vector2<float>(sector->coord.x*TILE_SIZE + sector->coord.x*GAP_SIZE, sector->coord.y*TILE_SIZE + sector->coord.y*GAP_SIZE));
                window->draw(spriteToDraw);
                if (piece->state == 'd') { // If done
                    // Draw the 'done' marker
                    this->gridSprite.setTextureRect(sf::Rect<int>(TILE_SIZE, TILE_SIZE, TILE_SIZE, TILE_SIZE));
                    this->gridSprite.setPosition(sector->coord.x*TILE_SIZE + sector->coord.x*GAP_SIZE, sector->coord.y*TILE_SIZE + sector->coord.y*GAP_SIZE);
                    window->draw(this->gridSprite);
                }
            } else {  // Draw tail sprite
                spriteToDraw.setTextureRect(sf::Rect<int>(0, 0, TILE_SIZE, TILE_SIZE));
                spriteToDraw.setPosition(sf::Vector2<float>(sector->coord.x*TILE_SIZE + sector->coord.x*GAP_SIZE, sector->coord.y*TILE_SIZE + sector->coord.y*GAP_SIZE));
                window->draw(spriteToDraw);
            }
            // Draw the collar sprite here (with color)
            spriteToDraw.setTextureRect(sf::Rect<int>(0, piece->spriteCoord.y*TILE_SIZE, TILE_SIZE, TILE_SIZE));
            spriteToDraw.setColor(this->db->players[piece->controller]->color);
            window->draw(spriteToDraw);
            spriteToDraw.setColor(sf::Color::White);  // Reset the color for when we use this next
        }
    }
    //cout << "Pieces drawn\n";

    // Upload phase: draw pieces in uploadMap
    if (this->db->currentPlayerIndex == -1) {
        for (pair<string, DataBattlePiece*> p : this->localPlayer->uploadMap) {
            //cout << "Drawing " << p.first << ' ' << p.second->uploadName << '\n';
            // Do something, Taipu
            sf::Vector2i tileCoord = readByteCoord(p.first);
            DataBattlePiece* piece = p.second;
            // Draw the head sprite for that piece at that tile
            //cout << "(" << piece->spriteCoord.x << ", " << piece->spriteCoord.y << ")\n";
            this->programSprite.setTextureRect(sf::Rect<int>(piece->spriteCoord.x*TILE_SIZE, piece->spriteCoord.y*TILE_SIZE, TILE_SIZE, TILE_SIZE));
            this->programSprite.setPosition(tileCoord.x * (TILE_SIZE + GAP_SIZE), tileCoord.y * (TILE_SIZE + GAP_SIZE));
            window->draw(this->programSprite);
            //cout << "Done drawing\n";
        }
    }
    //cout << "uploadMap drawn\n";

    // Draw the tile-highlighting cursor
    if (cursorTile.x != -1) {  // If we found a valid tile to highlight
        this->gridSprite.setTextureRect(sf::Rect<int>(0, 3*TILE_SIZE, TILE_SIZE, TILE_SIZE));
        this->gridSprite.setPosition(sf::Vector2<float>(cursorTile.x*TILE_SIZE + cursorTile.x*GAP_SIZE, cursorTile.y*TILE_SIZE + cursorTile.y*GAP_SIZE));
        window->draw(gridSprite);
    }
    //cout << "Cursor drawn\n";

    // Draw the HUD panel
    this->hudPanel.clear(sf::Color::Transparent);
    int i = 0;
    for (pair<string, int> p : localPlayer->programs) {
        //cout << p.first << '\n';
        this->hudText.setString(p.first + "  x" + to_string(p.second));
        this->hudText.setPosition(0, i*14);
        this->hudText.setColor(sf::Color::White);
        this->hudPanel.draw(this->hudText);
        i++;
    }
    this->hudText.setString("DATABATTLE INITIALIZE");
    this->hudText.setPosition(0, WY-14);
    this->hudPanel.draw(this->hudText);
    //cout << "HUD drawn\n";

    // Finally, if there's an InputBox on-screen, render it last over everything else
    if (this->inputBox != nullptr) {
        this->inputBox->render(window, this);
    }

    this->hudPanel.display();
    sf::Sprite sprite(this->hudPanel.getTexture());
    sprite.setPosition(WY, 0);
    window->draw(sprite);
}



string DataBattlePlayer::play(sf::RenderWindow* window) {
    cout << "Called DataBattlePlayer::play()\n";
    this->gridSprite = sf::Sprite(GRID_SHEET);
    this->programSprite = sf::Sprite(PROGRAM_SHEET);

    // Music
    if (!this->musicTrack.openFromFile("Data\\Music\\" + this->musicFilename)) {
        cout << "Music file not found: " << this->musicFilename << '\n';
    } else {
        musicTrack.setLoop(true);
        musicTrack.play();
    }

    // Background
    sf::Texture bkgTexture = imgLoad("Data\\DataBattleBkgs\\" + this->db->bkgFilename);
    this->bkgSprite = sf::Sprite(bkgTexture);

    // FPS measurements
    sf::Clock chrono;
    float lastTime = 0;
    float currentTime;
    float fps;
    int frameTimer;

    bool clicked;
    sf::Vector2i tileCoord(-1, -1);
    sf::IntRect hudButtonRect = sf::Rect<int>(WY, 0, 100, 14);

    bool pressedUp, pressedDown, pressedLeft, pressedRight, pressedN;

    while (window->isOpen()) {
        //cout << "DataBattlePlayer::play() loop\n";
        window->clear();
        this->mousePos = sf::Mouse::getPosition(*window);
        clicked = false;

        pressedUp = false;
        pressedDown = false;
        pressedLeft = false;
        pressedRight = false;
        pressedN = false;

        if (this->db->localPlayerIndex != -1) {
            this->localPlayer = this->db->players[this->db->localPlayerIndex];
        }

        if (this->inputBox != nullptr) {
            if (this->inputBox->done) {
                if (this->inputBoxType == 't') {  // Chat
                    string message = this->inputBox->getFocus();
                    this->localPlayer->cmdQueue.push("chat:" + message);
                    delete this->inputBox;
                    this->inputBox = nullptr;
                    this->inputBoxType = '0';
                } else if (this->inputBoxType == 'c') {  // Character
                    // Do something about setting the character here
                    delete this->inputBox;
                    this->inputBox = new ShopInputBox();  // Switch to shop screen
                    this->inputBoxType = 's';
                } else if (this->inputBoxType == 's') {  // Shop
                    delete this->inputBox;
                    this->inputBox = nullptr;
                    this->inputBoxType = '0';
                }
                // We should switch input boxes, say from Character -> Shop
            }
        }

        if (mousePos.x <= WY) {
            if ((mousePos.x % (TILE_SIZE + GAP_SIZE) <= TILE_SIZE) && (mousePos.y % (TILE_SIZE + GAP_SIZE) <= TILE_SIZE)) {  // If on a valid coord
                tileCoord.x = mousePos.x / (TILE_SIZE + GAP_SIZE);
                tileCoord.y = mousePos.y / (TILE_SIZE + GAP_SIZE);
            }
        } else {
            tileCoord.x = -1;
            tileCoord.y = -1;
        }

        // FPS measurements
        currentTime = chrono.restart().asSeconds();
        fps = 1.0 / (currentTime - lastTime);
        lastTime = currentTime;

        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window->close();

            if (this->inputBox != nullptr) {
                this->inputBox->setMousePos(mousePos);
                this->inputBox->takeInput(event, this);
            } else {
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::I) {
                        cout << fps << '\n';
                        //cout << "Victory status: " << this->checkForVictory() << '\n';
                        cout << "Current program: " << this->db->currentProgram->name << '\n';
                        for (ProgramSector* s : this->db->currentProgram->sectors) {
                            cout << "Sector: " << getByteCoord(s->coord) << '\n';
                            for (ProgramSector* l : s->links) {
                                cout << "Link: " << getByteCoord(l->coord) << '\n';
                            }
                        }
                    } else if (event.key.code == sf::Keyboard::Up) {
                        pressedUp = true;
                    } else if (event.key.code == sf::Keyboard::Down) {
                        pressedDown = true;
                    } else if (event.key.code == sf::Keyboard::Left) {
                        pressedLeft = true;
                    } else if (event.key.code == sf::Keyboard::Right) {
                        pressedRight = true;
                    } else if (event.key.code == sf::Keyboard::N) {  // No action
                        pressedN = true;
                    } else if (event.key.code == sf::Keyboard::T) {  // Chat
                        this->inputBox = new TextInputBox("Chat message:");
                        this->inputBoxType = 't';
                    }
                } else if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        clicked = true;
                    }
                }
            }
        }

        if (this->db->currentPlayerIndex == -1) {  // Upload phase
            if (clicked) {
                if ((tileCoord.x != -1) && (tileCoord.y != -1)) {  // See if we clicked on a tile
                    cout << "Valid coord: " << getByteCoord(tileCoord) << '\n';
                    // See if there's an upload zone there, set it as our selected upload
                    this->selectedUpload = nullptr;
                    for (DataBattlePiece* piece : this->db->pieces) {
                        if (piece->pieceType == 'u' && piece->owner == this->db->localPlayerIndex) {
                            if ((piece->sectors[0]->coord.x == tileCoord.x) && (piece->sectors[0]->coord.y == tileCoord.y)) {
                                this->selectedUpload = piece;
                                cout << "Clicked an upload zone\n";
                                break;
                            }
                        }
                    }
                } else if (mousePos.x >= WY) {  // We clicked in the HUD bar
                    // See if we clicked on one of our programs in the HUD bar
                    DataBattlePiece* selectedProgram = nullptr;
                    int i=0;
                    for (pair<string, int> p : this->localPlayer->programs) {
                        hudButtonRect.top = i*14;
                        if (hudButtonRect.contains(this->mousePos)) {
                            cout << "Clicked " << p.first << '\n';
                            if (selectedUpload != nullptr) {
                                this->localPlayer->addToUploadMap(getByteCoord(selectedUpload->sectors[0]->coord), p.first);
                            }
                            break;
                        }
                        i++;
                    }
                    // DataBattle Initialize
                    if (mousePos.y >= WY - 14) {  // If clicked DBI
                        cout << "Clicked DBI\n";
                        // Send our upload commands
                        for (pair<string, DataBattlePiece*> p : this->localPlayer->uploadMap) {
                            this->localPlayer->cmdQueue.emplace("upload:" + to_string(this->db->localPlayerIndex) + ":" + p.first + ":" + p.second->uploadName + ":NULL");
                        }
                        // Send ready signal
                        this->localPlayer->cmdQueue.emplace("DBI");
                    }
                }
            }
        }

        if ((this->db->currentPlayerIndex == this->db->localPlayerIndex) && (this->db->localPlayerIndex != -1)) {  // If it's our turn
            // Do something, Taipu
            if (clicked) {
                if (this->db->currentProgram->state == 'a') {  // If our program is aiming
                    if ((tileCoord.x != -1) && (tileCoord.y != -1)) {
                        this->localPlayer->cmdQueue.push("action:" + this->db->currentProgram->name + ":0:" + getByteCoord(tileCoord));
                    }
                }
            }
            if (pressedUp) {
                this->localPlayer->cmdQueue.push("move:" + this->db->currentProgram->name + ":n");
            } else if (pressedDown) {
                this->localPlayer->cmdQueue.push("move:" + this->db->currentProgram->name + ":s");
            } else if (pressedLeft) {
                this->localPlayer->cmdQueue.push("move:" + this->db->currentProgram->name + ":w");
            } else if (pressedRight) {
                this->localPlayer->cmdQueue.push("move:" + this->db->currentProgram->name + ":e");
            } else if (pressedN) {
                this->localPlayer->cmdQueue.push("noaction:" + this->db->currentProgram->name);
            }
        }

        //cout << "Calling tick()\n";
        this->db->tick();  // Tick the DB

        //cout << "Calling render()\n";
        this->render(window);
        window->display();
        //cout << "Finished main loop\n";
    }

    return "quit:";
}

void DataBattlePlayer::setDB(DataBattle* db) {
    this->db = db;
    this->musicFilename = this->db->musicFilename;
}

void DataBattlePlayer::setPlayer(Player* player) {
    this->localPlayer = player;
}
