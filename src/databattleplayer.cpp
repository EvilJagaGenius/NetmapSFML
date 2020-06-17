#include "databattleplayer.h"

DataBattlePlayer::DataBattlePlayer() {
    this->hudPanel.create(WY, WY);

    this->hudText = sf::Text("", DEFAULT_FONT, 12);
    this->hudText.setColor(sf::Color::White);
    //this->hudText.setCharacterSize(14);

    this->hudButton.setFillColor(sf::Color::Transparent);
    this->hudButton.setOutlineColor(sf::Color::White);
    this->hudButton.setOutlineThickness(2);
}

DataBattlePlayer::DataBattlePlayer(DataBattle* db) {
    this->hudPanel.create(WY, WY);

    this->hudText = sf::Text("", DEFAULT_FONT, 12);
    this->hudText.setColor(sf::Color::White);
    //this->hudText.setCharacterSize(14);

    this->hudButton.setFillColor(sf::Color::Transparent);
    this->hudButton.setOutlineColor(sf::Color::White);
    this->hudButton.setOutlineThickness(2);

    this->setDB(db);
}

DataBattlePlayer::~DataBattlePlayer()
{
    //dtor
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

    /*
    // Draw defenders
    this->programSprite.setColor(sf::Color::White);
    for (pair<string, DataBattlePiece*> element : defenders) {
        DataBattlePiece* p = element.second;
        if (p->visible) {
            for (int i=0; i<(p->size); i++) {
                ProgramSector* sector = p->sectors[i];
                // Draw connecting lines
                for (ProgramSector* s : sector->links) {
                    sf::Vector2i nextCoord = s->coord;
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
                    if (p->state == 'd') { // If done
                        // Draw the 'done' marker
                        this->gridSprite.setTextureRect(sf::Rect<int>(TILE_SIZE, TILE_SIZE, TILE_SIZE, TILE_SIZE));
                        this->gridSprite.setPosition(sector->coord.x*TILE_SIZE + sector->coord.x*GAP_SIZE, sector->coord.y*TILE_SIZE + sector->coord.y*GAP_SIZE);
                        window->draw(this->gridSprite);
                    }
                } else {  // Draw tail sprite
                    this->programSprite.setTextureRect(sf::Rect<int>(p->spriteCoord.x*TILE_SIZE, (p->spriteCoord.y + 1)*TILE_SIZE, TILE_SIZE, TILE_SIZE));
                    this->programSprite.setPosition(sector->coord.x*TILE_SIZE + sector->coord.x*GAP_SIZE, sector->coord.y*TILE_SIZE + sector->coord.y*GAP_SIZE);
                    window->draw(this->programSprite);
                }
            }
        }
    }

    // Draw friendlies
    this->programSprite.setColor(sf::Color::White);
    for (int i=0; i<this->friendlies.size(); i++) {
        DataBattlePiece* p = this->friendlies[i];
        if (!p->visible) {
            this->programSprite.setColor(sf::Color(128,128,128,128));  // Hopefully this turns things transparent
        } else {
            this->programSprite.setColor(sf::Color::White);  // And this fixes it
        }
        for (int j=0; j<(p->size); j++) {
            ProgramSector* sector = p->sectors[j];
            for (ProgramSector* s : sector->links) {
                sf::Vector2i nextCoord = s->coord;
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
                if (p->state == 'd') { // If done
                    // Draw the 'done' marker
                    this->gridSprite.setTextureRect(sf::Rect<int>(TILE_SIZE, TILE_SIZE, TILE_SIZE, TILE_SIZE));
                    this->gridSprite.setPosition(sector->coord.x*TILE_SIZE + sector->coord.x*GAP_SIZE, sector->coord.y*TILE_SIZE + sector->coord.y*GAP_SIZE);
                    window->draw(this->gridSprite);
                }
            } else {  // Draw tail sprite
                this->programSprite.setTextureRect(sf::Rect<int>(p->spriteCoord.x*TILE_SIZE, (p->spriteCoord.y + 1)*TILE_SIZE, TILE_SIZE, TILE_SIZE));
                this->programSprite.setPosition(sf::Vector2<float>(sector->coord.x*TILE_SIZE + sector->coord.x*GAP_SIZE, sector->coord.y*TILE_SIZE + sector->coord.y*GAP_SIZE));
                window->draw(this->programSprite);
            }
        }
    }

    // Draw upload zones
    if (this->phase == 'u') {  // If in the upload phase
        for (int i = 0; i < this->uploads.size(); i++) {
            sf::Vector2i coord = this->uploads[i];
            this->gridSprite.setPosition(coord.x*TILE_SIZE + coord.x*GAP_SIZE, coord.y*TILE_SIZE + coord.y*GAP_SIZE);
            if (i == this->selectedUpload) {
                this->gridSprite.setTextureRect(sf::Rect<int>(1*TILE_SIZE, 2*TILE_SIZE, TILE_SIZE, TILE_SIZE));
            } else {
                this->gridSprite.setTextureRect(sf::Rect<int>(0, 2*TILE_SIZE, TILE_SIZE, TILE_SIZE));
            }
            window->draw(this->gridSprite);
        }
    } else {
    // Draw movement area, associated buttons
    if (db->currentProgram->state == 'm') {
        for (sf::Vector2i coord : this->moveArea) {
            if ((this->grid[coord.x][coord.y]) != 0 || (this->db->currentProgram->statuses['g'] != 0)) {  // We shouldn't be running this check here, the DB should
                if (coord == this->nButton) {
                    this->gridSprite.setTextureRect(sf::Rect<int>(0*TILE_SIZE, 4*TILE_SIZE, TILE_SIZE, TILE_SIZE));
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
    } else if (db->currentProgram->state == 'a') {  // Draw aiming area
        for (sf::Vector2i coord : this->aimArea) {
            this->gridSprite.setTextureRect(sf::Rect<int>(this->db->currentProgram->currentAction->targetSprite*TILE_SIZE, 5*TILE_SIZE, TILE_SIZE, TILE_SIZE));
            this->gridSprite.setPosition(coord.x * (TILE_SIZE + GAP_SIZE), coord.y * (TILE_SIZE + GAP_SIZE));
            window->draw(this->gridSprite);
        }
    }*/

    // Draw pieces
    for (int i=0; i<this->db->pieces.size(); i++) {
        //cout << "Looping through pieces\n";
        DataBattlePiece* piece = this->db->pieces[i];
        sf::Sprite spriteToDraw;
        spriteToDraw = this->gridSprite;
        if (piece->pieceType == 'u') {  // Upload zone
            spriteToDraw = this->gridSprite;
        } else {  // Anything else
            spriteToDraw = this->programSprite;
        }

        /*if (!piece->visible) {
            // If we control the piece
                this->programSprite.setColor(sf::Color(128,128,128,128));  // Hopefully this turns things transparent
            // Else
                //this->programSprite.setColor(sf::Color::Transparent);
        } else {
            this->programSprite.setColor(sf::Color::White);  // And this fixes it
        }*/
        /*// Draw connecting lines
        for (int j=0; j<(piece->size); j++) {
            //cout << "Looping through sectors\n";
            ProgramSector* sector = piece->sectors[j];
            for (ProgramSector* s : sector->links) {
                sf::Vector2i nextCoord = s->coord;
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
                connectingLine.setFillColor(piece->color);
                connectingLine.setOutlineColor(piece->color);
                connectingLine.setPosition(topLeft);
                window->draw(connectingLine);
            }
        }*/
        // Draw sector sprites
        for (int j=0; j<(piece->size); j++) {
            ProgramSector* sector = piece->sectors[j];
            if (j==0) {  // Draw head sprite
                //cout << "Drawing head sprite\n";
                spriteToDraw.setTextureRect(sf::Rect<int>(piece->spriteCoord.x*TILE_SIZE, piece->spriteCoord.y*TILE_SIZE, TILE_SIZE, TILE_SIZE));
                spriteToDraw.setPosition(sf::Vector2<float>(sector->coord.x*TILE_SIZE + sector->coord.x*GAP_SIZE, sector->coord.y*TILE_SIZE + sector->coord.y*GAP_SIZE));
                window->draw(spriteToDraw);
                if (piece->state == 'd') { // If done
                    // Draw the 'done' marker
                    this->gridSprite.setTextureRect(sf::Rect<int>(TILE_SIZE, TILE_SIZE, TILE_SIZE, TILE_SIZE));
                    this->gridSprite.setPosition(sector->coord.x*TILE_SIZE + sector->coord.x*GAP_SIZE, sector->coord.y*TILE_SIZE + sector->coord.y*GAP_SIZE);
                    window->draw(this->gridSprite);
                }
            } else {  // Draw tail sprite
                spriteToDraw.setTextureRect(sf::Rect<int>(piece->spriteCoord.x*TILE_SIZE, (piece->spriteCoord.y + 1)*TILE_SIZE, TILE_SIZE, TILE_SIZE));
                spriteToDraw.setPosition(sf::Vector2<float>(sector->coord.x*TILE_SIZE + sector->coord.x*GAP_SIZE, sector->coord.y*TILE_SIZE + sector->coord.y*GAP_SIZE));
                window->draw(spriteToDraw);
            }
        }
    }

    // Last: Draw the tile-highlighting cursor
    if (cursorTile.x != -1) {  // If we found a valid tile to highlight
        this->gridSprite.setTextureRect(sf::Rect<int>(0, 3*TILE_SIZE, TILE_SIZE, TILE_SIZE));
        this->gridSprite.setPosition(sf::Vector2<float>(cursorTile.x*TILE_SIZE + cursorTile.x*GAP_SIZE, cursorTile.y*TILE_SIZE + cursorTile.y*GAP_SIZE));
        window->draw(gridSprite);
    }

    // Draw the HUD panel
    sf::Font font = fontLoad("Data\\Fonts\\Terminus.ttf");

    this->hudPanel.clear(sf::Color::Transparent);
    Player* localPlayer = this->db->players[localPlayerIndex];
    int i = 0;
    for (pair<string, int> p : localPlayer->programs) {
        //cout << p.first << '\n';
        this->hudText.setString(p.first);
        this->hudText.setPosition(0, i*14);
        this->hudText.setColor(sf::Color::White);
        //window->draw(this->hudText);
        this->hudPanel.draw(this->hudText);
        //renderText(&this->hudPanel, p.first, sf::Rect<int>(0,0,100,100), this->hudFont, 14, sf::Color::Black);

        //this->hudPanel.draw(rect);
        //window->draw(rect);
        i++;
    }
    this->hudPanel.display();
    sf::Sprite sprite(this->hudPanel.getTexture());
    sprite.setPosition(WY, 0);
    //this->hudSprite.setTexture(this->hudPanel.getTexture());
    //this->hudSprite.setTextureRect(sf::Rect<int>(100, 100, 100, 100));
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

    // FPS measurements
    sf::Clock chrono;
    float lastTime = 0;
    float currentTime;
    float fps;
    int frameTimer;

    bool clicked;
    sf::Vector2i tileCoord(-1, -1);
    sf::IntRect hudButtonRect = sf::Rect<int>(WY, 0, 100, 14);

    while (window->isOpen()) {
        window->clear();
        this->mousePos = sf::Mouse::getPosition(*window);
        clicked = false;

        if (mousePos.x <= WY) {
            if ((mousePos.x % (TILE_SIZE + GAP_SIZE) <= TILE_SIZE) && (mousePos.y % (TILE_SIZE + GAP_SIZE) <= TILE_SIZE)) {  // If on a valid coord
                tileCoord.x = mousePos.x / (TILE_SIZE + GAP_SIZE);
                tileCoord.y = mousePos.y / (TILE_SIZE + GAP_SIZE);
            } else {
                tileCoord.x = -1;
                tileCoord.y = -1;
            }
        }

        // FPS measurements
        currentTime = chrono.restart().asSeconds();
        fps = 1.0 / (currentTime - lastTime);
        lastTime = currentTime;

        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window->close();

            if (event.type == sf::Event::KeyPressed) {
                cout << fps << '\n';
                //cout << "Victory status: " << this->checkForVictory() << '\n';
                cout << "Current program: " << this->db->currentProgram->name << '\n';
                for (ProgramSector* s : this->db->currentProgram->sectors) {
                    cout << "Sector: " << getByteCoord(s->coord) << '\n';
                    for (ProgramSector* l : s->links) {
                        cout << "Link: " << getByteCoord(l->coord) << '\n';
                    }
                }
            } else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    clicked = true;
                }
            }
        }

        if (this->db->currentPlayerIndex == -1) {  // Upload phase
            // Do something, Taipu
            if (clicked) {
                if ((tileCoord.x != -1) && (tileCoord.y != -1)) {  // See if we clicked on a tile
                    cout << "Valid coord\n";
                    // See if there's an upload zone there, set it as our selected upload
                    this->selectedUpload = nullptr;
                    for (DataBattlePiece* piece : this->db->pieces) {
                        if (piece->pieceType == 'u') {
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
                    for (pair<string, int> p : this->db->players[localPlayerIndex]->programs) {
                        hudButtonRect.top = i*14;
                        if (hudButtonRect.contains(this->mousePos)) {
                            cout << "Clicked " << p.first << '\n';
                            if (selectedUpload != nullptr) {
                                // Do something, Taipu
                                // Put that program in uploadMap, so we upload it on hitting DBI
                                foundUpload = false;
                                for (pair<string, string> p2 : this->uploadMap) {
                                    if (p2.first == getByteCoord(selectedUpload->sectors[0]->coord)) {
                                        foundUpload = true;
                                        p2.second = p.first;
                                        break;
                                    }
                                }
                                if (!foundUpload) {
                                    this->uploadMap.emplace(getByteCoord(selectedUpload->sectors[0]->coord), p.first);
                                }
                                // Note: add code to render() to render pieces in the upload map
                            }
                            break;
                        }
                        i++;
                    }

                    // We need a DBI button
                    /*
                    // If pressed DBI:
                    // Send our upload commands
                    for (pair<string, string> p : this->uploadMap) {
                        this->db->takeCommand(p.first + ":" + p.second + ":NULL", this->localPlayerIndex);
                    }
                    // Send ready signal
                    this->db->takeCommand("DBI", this->localPlayerIndex);
                    */
                }
            }
        }

        if (this->db->currentPlayerIndex == this->localPlayerIndex) {  // If it's our turn
            // Do something, Taipu
            if (clicked) {
                cout << "Player clicked\n";
            }
        }


        /*// Player's turn
        if (this->phase == 'p'  && this->currentProgram != nullptr) {
            this->programHead = this->currentProgram->sectors[0]->coord;

            if ((this->currentProgram->state == 'm') && (this->currentProgram->currentMove >= this->currentProgram->speed)) {  // If out of moves
                this->currentProgram->switchToAiming(0);
                // Get aim area here
            }

            if (clicked) {
                if (this->currentProgram->state == 'm') {  // If moving
                    this->nButton = sf::Vector2<int>(programHead.x, programHead.y - 1);
                    this->sButton = sf::Vector2<int>(programHead.x, programHead.y + 1);
                    this->eButton = sf::Vector2<int>(programHead.x + 1, programHead.y);
                    this->wButton = sf::Vector2<int>(programHead.x - 1, programHead.y);

                    if ((this->grid[tileCoord.x][tileCoord.y] != 0) || (this->currentProgram->statuses['g'] != 0)) {  // If it's a tile (or we have gridwalk)
                        if (tileCoord == nButton) {
                            //this->currentProgram->move(this, nButton, false);
                            //hud->setFocus(this->currentProgram);
                            db->takeCommand("move:" + to_string(currentProgramIndex) + ":n");
                        } else if (tileCoord == sButton) {
                            //this->currentProgram->move(this, sButton, false);
                            //hud->setFocus(this->currentProgram);
                            db->takeCommand("move:" + to_string(currentProgramIndex) + ":s");
                        } else if (tileCoord == eButton) {
                            //this->currentProgram->move(this, eButton, false);
                            //hud->setFocus(this->currentProgram);
                            db->takeCommand("move:" + to_string(currentProgramIndex) + ":e");
                        } else if (tileCoord == wButton) {
                            //this->currentProgram->move(this, wButton, false);
                            //hud->setFocus(this->currentProgram);
                            db->takeCommand("move:" + to_string(currentProgramIndex) + ":w");
                        }
                    }

                    this->programHead = this->currentProgram->sectors[0]->coord;
                    if (this->currentProgram->currentMove < this->currentProgram->speed) {
                        this->moveArea = getRadius(this->currentProgram->speed - this->currentProgram->currentMove, this->programHead, false);

                        /*this->nButton = sf::Vector2<int>(programHead.x, programHead.y - 1);
                        this->sButton = sf::Vector2<int>(programHead.x, programHead.y + 1);
                        this->eButton = sf::Vector2<int>(programHead.x + 1, programHead.y);
                        this->wButton = sf::Vector2<int>(programHead.x - 1, programHead.y);
                    } else {
                        this->targets.clear();
                        this->currentProgram->switchToAiming(0);
                        this->aimArea = this->currentProgram->currentAction->getAimArea(this->programHead, this->targets.size());
                        //this->hud->setSubFocus(0);
                    }

                } else if (this->currentProgram->state == 'a') {  // If aiming
                    for (sf::Vector2i coord : this->aimArea) {
                        if (coord == tileCoord) {
                            this->targets.push_back(tileCoord);  // Add it to the target list
                            this->aimArea = this->currentProgram->currentAction->getAimArea(this->programHead, this->targets.size());
                            if (this->targets.size() >= this->currentProgram->currentAction->numOfTargets) {  // If we've selected all our targets
                                //this->currentProgram->useAction(this, this->currentProgram->currentActionIndex, this->targets);
                                // Switch to next available program
                                this->switchPrograms();
                                string commandString = "action" + to_string(currentProgram->currentActionIndex);
                                for (sf::Vector2i coord : this->targets) {
                                    commandString += ":" + getByteCoord(coord);
                                }
                                db->takeCommand(commandString);
                            }
                        }
                    }
                }
            }
        }

        // Enemy turn
        else if (this->phase == 'e') {
            if (frameTimer >= 10) {
                frameTimer = 0;
                programHead = this->currentProgram->sectors[0]->coord;
                // Rudimentary AI here, copied from the Python.  If somebody could help me improve this that'd be nice
                // Look for closest target
                sf::Vector2i targetCoord = sf::Vector2<int>(-1, -1);
                for (int i=0; i<this->friendlies.size(); i++) {
                    for (ProgramSector* s : this->friendlies[i]->sectors) {
                        if (targetCoord.x == -1) {  // If we haven't set targetCoord yet
                            targetCoord = s->coord;
                        } else {
                            int distanceToTarget = abs(targetCoord.x - programHead.x) + abs(targetCoord.y - programHead.y);
                            int distanceToCandidate = abs(s->coord.x - programHead.x) + abs(s->coord.y - programHead.y);
                            if (distanceToCandidate < distanceToTarget) {  // If that target is closer
                                targetCoord = s->coord;  // Change targets
                            }
                        }
                    }
                }
                int xDistance = targetCoord.x - programHead.x;
                int yDistance = targetCoord.y - programHead.y;

                if (this->currentProgram->state == 'm') {  // If moving
                    if (this->currentProgram->currentMove >= this->currentProgram->speed) {  // If it's out of moves
                        this->currentProgram->switchToAiming(0);
                        this->targets.clear();
                        this->aimArea = this->currentProgram->currentAction->getAimArea(this->currentProgram->sectors[0]->coord, this->targets.size());
                    } else if ((yDistance < 0) && ((startsWith(this->lookAt(programHead.x, programHead.y-1), "tile")) || (startsWith(this->lookAt(programHead.x, programHead.y-1), "defender " + this->currentDefenderIndex)))) {
                        // North
                        this->currentProgram->move(this, sf::Vector2<int>(programHead.x, programHead.y-1), false);
                        this->programHead = this->currentProgram->sectors[0]->coord;
                        this->moveArea = getRadius(this->currentProgram->speed, this->programHead);
                    } else if ((yDistance > 0) && ((startsWith(this->lookAt(programHead.x, programHead.y+1), "tile")) || (startsWith(this->lookAt(programHead.x, programHead.y+1), "defender " + this->currentDefenderIndex)))) {
                        // South
                        this->currentProgram->move(this, sf::Vector2<int>(programHead.x, programHead.y+1), false);
                        this->programHead = this->currentProgram->sectors[0]->coord;
                        this->moveArea = getRadius(this->currentProgram->speed, this->programHead);
                    } else if ((xDistance > 0) && ((startsWith(this->lookAt(programHead.x+1, programHead.y), "tile")) || (startsWith(this->lookAt(programHead.x+1, programHead.y), "defender " + this->currentDefenderIndex)))) {
                        // East
                        this->currentProgram->move(this, sf::Vector2<int>(programHead.x+1, programHead.y), false);
                        this->programHead = this->currentProgram->sectors[0]->coord;
                        this->moveArea = getRadius(this->currentProgram->speed, this->programHead);
                    } else if ((xDistance < 0) && ((startsWith(this->lookAt(programHead.x-1, programHead.y), "tile")) || (startsWith(this->lookAt(programHead.x-1, programHead.y), "defender " + this->currentDefenderIndex)))) {
                        // West
                        this->currentProgram->move(this, sf::Vector2<int>(programHead.x-1, programHead.y), false);
                        this->programHead = this->currentProgram->sectors[0]->coord;
                        this->moveArea = getRadius(this->currentProgram->speed, this->programHead);
                    } else {  // If you can't move anywhere you'd like to go
                        if ((xDistance == 0) || (yDistance == 0)) {  // If you're right against the target
                            this->currentProgram->switchToAiming(0);  // Switch to aiming
                            this->aimArea = this->currentProgram->currentAction->getAimArea(this->currentProgram->sectors[0]->coord, this->targets.size());
                        } else {
                            this->currentProgram->noAction();  // Take no action
                        }
                    }
                } else if (this->currentProgram->state == 'a') {  // If aiming
                    // Attempt to attack the closest sector of a friendly program
                    int actionIndex = 0;  // This value can be changed if the AI improves and defender Programs get multiple actions to choose from
                    this->targets.clear();
                    this->targets.push_back(targetCoord);
                    cout << "Attacking " << getByteCoord(targetCoord) << '\n';
                    this->currentProgram->useAction(this, actionIndex, targets);

                    // Switch to the next available defender; failing that, switch turns
                    this->switchPrograms(hud);
                }
            }
            frameTimer++;
        }*/

        // Check to see if anybody won, and if so, end the DataBattle
        // Wait, this check shouldn't be in here
        /*if ((this->db->currentPlayerIndex != -1) && (this->checkForVictory() != '0')) {
            if (this->checkForVictory() == 'p') {  // If the player won
                cout << "Databattle won\n";
                musicTrack.stop();
                return "";
            } else {  // If the computer won
                cout << "Databattle lost\n";
                musicTrack.stop();
                return this->destination;
            }
        }
        if (this->disconnect) {
            return this->destination;
        }*/

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
