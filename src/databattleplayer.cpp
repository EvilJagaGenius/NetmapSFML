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
    this->focusPiece = nullptr;
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
    this->focusPiece = nullptr;

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
    this->hudText.setColor(sf::Color(255, 255, 255, 127));
    for (int x=0; x<16; x++) {
        for (int y=0; y<16; y++) {
            this->hudText.setPosition(sf::Vector2<float>(x*TILE_SIZE + x*GAP_SIZE + 4, y*TILE_SIZE + y*GAP_SIZE + 4));
            this->hudText.setString(getByteCoord(sf::Vector2<int>(x, y)));
            window->draw(this->hudText);

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
                spriteToDraw.setTextureRect(sf::Rect<int>(1*TILE_SIZE, 0, TILE_SIZE, TILE_SIZE));
                spriteToDraw.setColor(piece->color);
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


    if (this->db->currentProgram != nullptr) {
        if (this->db->currentProgram->state == 'a') {  // Draw the aiming area
            for (sf::Vector2i coord : this->aimArea) {
                this->gridSprite.setTextureRect(sf::Rect<int>(0, 5*TILE_SIZE, TILE_SIZE, TILE_SIZE));
                int targetSprite = this->db->currentProgram->currentAction->targetSprite;
                if (targetSprite == 0) {  // Red, damage
                    this->gridSprite.setColor(sf::Color::Red);
                } else if (targetSprite == 1) {  // Green, grid
                    this->gridSprite.setColor(sf::Color::Green);
                } else if (targetSprite == 2) {  // Blue, buff/debuff
                    this->gridSprite.setColor(sf::Color::Blue);
                }
                this->gridSprite.setPosition(coord.x * (TILE_SIZE + GAP_SIZE), coord.y * (TILE_SIZE + GAP_SIZE));
                window->draw(this->gridSprite);
                this->gridSprite.setColor(sf::Color::White);
            }
        } else if (this->db->currentProgram->state == 'm') {  // Draw the movement area
            for (sf::Vector2i coord : this->moveArea) {
                if (coord == this->nButton) {
                    this->gridSprite.setTextureRect(sf::Rect<int>(0, 4*TILE_SIZE, TILE_SIZE, TILE_SIZE));
                } else if (coord == this->sButton) {
                    this->gridSprite.setTextureRect(sf::Rect<int>(1*TILE_SIZE, 4*TILE_SIZE, TILE_SIZE, TILE_SIZE));
                } else if (coord == this->eButton) {
                    this->gridSprite.setTextureRect(sf::Rect<int>(2*TILE_SIZE, 4*TILE_SIZE, TILE_SIZE, TILE_SIZE));
                } else if (coord == this->wButton) {
                    this->gridSprite.setTextureRect(sf::Rect<int>(3*TILE_SIZE, 4*TILE_SIZE, TILE_SIZE, TILE_SIZE));
                } else {
                    this->gridSprite.setTextureRect(sf::Rect<int>(3*TILE_SIZE, 5*TILE_SIZE, TILE_SIZE, TILE_SIZE));
                }
                this->gridSprite.setPosition(coord.x * (TILE_SIZE + GAP_SIZE), coord.y * (TILE_SIZE + GAP_SIZE));
                window->draw(this->gridSprite);
            }
        }
    }

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
        // This would be a really great spot to have a PROGRAM_DB again
        this->hudText.setString(p.first + "  x" + to_string(p.second));
        this->hudText.setPosition(0, i*14);
        this->hudText.setColor(sf::Color::White);
        this->hudPanel.draw(this->hudText);
        i++;
    }

    // Draw focus data
    if (this->focusPiece != nullptr) {
        if (this->focusType == 'u') {
            this->focusSprite.setTexture(GRID_SHEET);
        } else if (this->focusType == 'p') {
            this->focusSprite.setTexture(PROGRAM_SHEET);
            this->hudText.setPosition(220, 174);
            this->hudText.setString("Move:" + to_string(this->focusPiece->currentMove) + "/" + to_string(this->focusPiece->speed));
            this->hudPanel.draw(this->hudText);

            this->hudText.setPosition(220, 188);
            this->hudText.setString("Size:" + to_string(this->focusPiece->size) + "/" + to_string(this->focusPiece->maxSize));
            this->hudPanel.draw(this->hudText);

            this->hudText.setPosition(16, 200);
            for (ProgramAction* action : this->focusPiece->actions) {
                this->hudText.setString(action->actionName);
                if (action->checkPrereqs(this->focusPiece)) {
                    this->hudText.setColor(sf::Color::White);
                } else {
                    this->hudText.setColor(sf::Color::Red);
                }
                this->hudPanel.draw(this->hudText);
                this->hudText.setPosition(16, this->hudText.getPosition().y + 14);
            }
            this->hudText.setColor(sf::Color::Red);
            this->hudText.setString("NO ACTION");
            this->hudPanel.draw(this->hudText);
            this->hudText.setColor(sf::Color::White);
        }
        // Draw main sprite
        this->focusSprite.setTextureRect(sf::Rect<int>(this->focusPiece->spriteCoord.x*TILE_SIZE, this->focusPiece->spriteCoord.y*TILE_SIZE, TILE_SIZE, TILE_SIZE));
        this->focusSprite.setColor(sf::Color::White);
        this->hudPanel.draw(this->focusSprite);
        // Draw collar sprite
        this->focusSprite.setTextureRect(sf::Rect<int>(0, this->focusPiece->spriteCoord.y*TILE_SIZE, TILE_SIZE, TILE_SIZE));
        this->focusSprite.setPosition(16, 160);
        //this->focusSprite.setColor(this->db->players[this->focusPiece->controller]->color);
        this->hudPanel.draw(this->focusSprite);

        // Draw name
        this->hudText.setPosition(20 + TILE_SIZE, 160);
        this->hudText.setString(this->focusPiece->screenName + "@" + getByteCoord(this->focusCoord));
        this->hudPanel.draw(this->hudText);

        // Draw description
        this->hudText.setPosition(220, 160);
        this->hudText.setString(this->focusPiece->description);
        this->hudPanel.draw(this->hudText);
    }


    this->hudText.setString("DATABATTLE INITIALIZE");
    this->hudText.setPosition(0, WY-14);
    this->hudPanel.draw(this->hudText);
    //cout << "HUD drawn\n";

    this->hudPanel.display();
    sf::Sprite hudSprite(this->hudPanel.getTexture());
    hudSprite.setPosition(WY, 0);
    window->draw(hudSprite);

    // Finally, if there's an InputBox on-screen, render it last over everything else
    if (this->inputBox != nullptr) {
        this->inputBox->render(window, this);
    }
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
        //musicTrack.play();  // Silence
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
                } else if (this->inputBoxType == '`') {  // Console command
                    string command = this->inputBox->getFocus();
                    this->localPlayer->cmdQueue.push(command);
                    delete this->inputBox;
                    this->inputBox = nullptr;
                    this->inputBoxType = '0';
                } else if (this->inputBoxType == 'c') {  // Character
                    // Assuming this is just a normal ChoiceInputBox; I could program a specialized box
                    string characterName = this->inputBox->getFocus();
                    int characterIndex = this->inputBox->getSubFocus();
                    // Do something about setting the character here
                    this->localPlayer->cmdQueue.push("character:" + characterName);
                    delete this->inputBox;
                    this->inputBox = new ShopInputBox();  // Switch to shop screen
                    this->inputBoxType = 's';
                } else if (this->inputBoxType == 's') {  // Shop
                    delete this->inputBox;
                    this->inputBox = nullptr;
                    this->inputBoxType = '0';
                }
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
                        cout << "Victory status: " << this->db->checkForVictory() << '\n';
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
                    } else if (event.key.code == sf::Keyboard::Tilde) {  // Command console
                        this->inputBox = new TextInputBox("Console command:");
                        this->inputBoxType = '`';
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
                        if (piece->pieceType == 'u') {
                            if ((piece->sectors[0]->coord.x == tileCoord.x) && (piece->sectors[0]->coord.y == tileCoord.y)) {
                                this->focusType = 'u';
                                this->focusPiece = piece;
                                this->focusCoord = tileCoord;
                                if (piece->owner == this->db->localPlayerIndex) {
                                    cout << "Changed selectedUpload\n";
                                    this->selectedUpload = piece;
                                    break;
                                }
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
                                this->focusType = 'p';
                                this->focusPiece = this->localPlayer->uploadMap[getByteCoord(selectedUpload->sectors[0]->coord)];
                                this->focusCoord = selectedUpload->sectors[0]->coord;
                            }
                            break;
                        }
                        i++;
                    }
                    // DataBattle Initialize
                    if (mousePos.y >= WY - 14 && (this->localPlayer->uploadMap.size() > 0)) {  // If clicked DBI
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
            if (clicked) {
                // If we have a program focused, see if we clicked one of its actions
                if (this->focusType == 'p') {
                    sf::Rect<int> actionButtonRect = sf::Rect<int>(WY+16, 200, 100, 14);
                    for (int i=0; i<=this->focusPiece->actions.size(); i++) {
                        //ProgramAction* action = this->focusPiece->actions[i];
                        if (actionButtonRect.contains(mousePos)) {
                            this->subFocus = i;
                            if (this->focusPiece->controller == this->db->localPlayerIndex) {  // Only do important stuff like switching to aiming if we control that piece
                                if (i == this->focusPiece->actions.size()) {  // We clicked on No Action
                                    this->localPlayer->cmdQueue.emplace("noaction:" + this->focusPiece->name);
                                } else {
                                    this->focusPiece->switchToAiming(i);
                                    this->adjustRadii();
                                }
                            }
                        }
                        actionButtonRect.top += 14;
                    }
                }
                if (this->db->currentProgram->state == 'a') {  // If our program is aiming
                    if ((tileCoord.x != -1) && (tileCoord.y != -1)) {
                        this->localPlayer->cmdQueue.push("action:" + this->db->currentProgram->name + ":0:" + getByteCoord(tileCoord));
                    }
                }
            }
            if (pressedUp || (clicked && tileCoord == this->nButton)) {
                this->localPlayer->cmdQueue.push("move:" + this->db->currentProgram->name + ":n");
            } else if (pressedDown || (clicked && tileCoord == this->sButton)) {
                this->localPlayer->cmdQueue.push("move:" + this->db->currentProgram->name + ":s");
            } else if (pressedLeft || (clicked && tileCoord == this->eButton)) {
                this->localPlayer->cmdQueue.push("move:" + this->db->currentProgram->name + ":w");
            } else if (pressedRight || (clicked && tileCoord == this->wButton)) {
                this->localPlayer->cmdQueue.push("move:" + this->db->currentProgram->name + ":e");
            } else if (pressedN) {
                this->localPlayer->cmdQueue.push("noaction:" + this->db->currentProgram->name);
            }
        }

        //cout << "Calling tick()\n";
        this->db->tick();  // Tick the DB
        this->adjustRadii();
        // The block below is causing us to exit the DB early.  Need to fix
        /*if (this->db->checkForVictory() != -1) {  // If someone has won the DB (or we need to shut down... do we need a dedicated function for that?)
            return this->db->destination;  // Exit the DB
        }*/

        //cout << "Calling render()\n";
        this->render(window);
        window->display();
        //cout << "Finished main loop\n";
    }

    return "quit:";
}

void DataBattlePlayer::adjustRadii() {
    this->focusPiece = this->db->currentProgram;
    if (this->db->currentProgram != nullptr) {
        if (this->db->currentProgram->state == 'm') {
            int movesRemaining = this->db->currentProgram->speed - this->db->currentProgram->currentMove;
            this->moveArea = getRadius(movesRemaining, this->db->currentProgram->sectors[0]->coord, false);
        } else if (this->db->currentProgram->state == 'a') {
            if (this->db->currentProgram->currentAction != nullptr) {
                this->aimArea = this->db->currentProgram->currentAction->getAimArea(this->db->currentProgram->sectors[0]->coord, 0);
            }
        }
    }
}

void DataBattlePlayer::setDB(DataBattle* db) {
    this->db = db;
    this->musicFilename = this->db->musicFilename;
}

void DataBattlePlayer::setPlayer(Player* player) {
    this->localPlayer = player;
}
