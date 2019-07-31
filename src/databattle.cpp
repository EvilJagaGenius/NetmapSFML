#include "databattle.h"

DataBattle::DataBattle() {

}

DataBattle::DataBattle(string filename)
{
    this->playableType = 'd';

    this->filename = filename;

    this->friendliesLoaded = 0;
    this->programStartingState = nullptr;


    for (int i=0; i<16; i++) {
        for (int j=0; j<16; j++) {
            this->grid[i][j] = 0;
        }
    }

    this->load();
}

DataBattle::~DataBattle() {
    // Remember to fill this in, deallocate your programs.
    for (int i=0; i<this->friendlies.size(); i++) {
        delete this->friendlies[i];
    }
    for (pair<string, DataBattlePiece*> p : this->defenders) {
        delete this->defenders[p.first];
    }
}

void DataBattle::load() {
    ifstream textFile;
    textFile.open("Data\\DataBattles\\" + filename + ".txt");
    string line;
    char loading = '0';  // 0 if we're not loading anything

    int gridY = 0;

    while(getline(textFile, line)) {
        if (startsWith(line, "bkg:")) {  // Background
            this->bkgFilename = line.substr(4);
            this->bkg = imgLoad("Data\\DataBattleBkgs\\" + bkgFilename);
            this->bkgSprite = sf::Sprite(this->bkg);
        }
        if (startsWith(line, "cash:")) {  // Cash reward
            this->cashReward = stoi(line.substr(5));
        }
        if (startsWith(line, "music:")) {  // Music
            musicFilename = line.substr(6);
        }

        // Grid
        if (startsWith(line, "start_grid")) {
            loading = 'G';
        } if (loading == 'G') {
            if (startsWith(line, "start_grid")) {
                continue;
            } else if (startsWith(line, "end_grid")) {
                loading = '0';
            } else {
                int gridX = 0;
                for (char c : line) {
                    this->grid[gridX][gridY] = c - '0';
                    gridX++;
                }
                gridY++;
            }
        }

        // Commands
        if (startsWith(line, "start_commands")) {
            loading = 'C';
        } if (loading == 'C') {
            if (startsWith(line, "start_commands")) {
                continue;
            } else if (startsWith(line, "end_commands")) {
                loading = '0';
            } else {
                if (startsWith(line, "addUpload")) {  // Upload zone
                    vector<string> splitLine = splitString(line, ':');
                    sf::Vector2<int> coord(stoi(splitLine[1]), stoi(splitLine[2]));
                    this->uploads.push_back(coord);
                }

                if (startsWith(line, "addDefender")) {  // Enemy program
                    // 1:DataBattlePiece type, 2:x, 3:y, 4:Reference name
                    vector<string> splitLine = splitString(line, ':');
                    Program* newProgram = new Program(splitLine[1]);
                    newProgram->move(sf::Vector2<int>(stoi(splitLine[2]), stoi(splitLine[3])), true);
                    newProgram->owner = 'c';  // Computer owns the program
                    this->defenders.insert({{splitLine[4], newProgram}});
                }

                if (startsWith(line, "addSector")) {
                    vector<string> splitLine = splitString(line, ':');
                    DataBattlePiece* targetProgram = this->defenders[splitLine[1]];
                    int targetSector = stoi(splitLine[4]);
                    targetProgram->addSector(sf::Vector2<int>(stoi(splitLine[2]), stoi(splitLine[3])), targetSector);
                }

            }
        }
    }
    textFile.close();
}

void DataBattle::render(sf::RenderWindow* window) {
    //cout << "Rendering\n";
    // Draw the background
    window->draw(this->bkgSprite);
    sf::Vector2<int> cursorTile(-1, -1);
    //cout << "Background\n";

    // Draw the grid
    for (int x=0; x<16; x++) {
        for (int y=0; y<16; y++) {
            sf::Rect<int> tileRect(x*TILE_SIZE + x*GAP_SIZE, y*TILE_SIZE + y*GAP_SIZE, TILE_SIZE, TILE_SIZE);
            this->gridSprite.setTextureRect(sf::Rect<int>(0, this->grid[x][y]*TILE_SIZE, TILE_SIZE, TILE_SIZE));
            this->gridSprite.setPosition(sf::Vector2<float>(x*TILE_SIZE + x*GAP_SIZE, y*TILE_SIZE + y*GAP_SIZE));
            window->draw(this->gridSprite);

            if (tileRect.contains(this->mousePos)) {
                cursorTile = sf::Vector2<int>(x, y);
            }
        }
    }
    //cout << "Grid\n";

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
        if (this->currentProgram->state == 'm') {
            for (sf::Vector2i coord : this->moveArea) {
                if (this->grid[coord.x][coord.y] != 0) {
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
        } else if (this->currentProgram->state == 'a') {
            for (sf::Vector2i coord : this->aimArea) {
                this->gridSprite.setTextureRect(sf::Rect<int>(this->currentProgram->currentAction->targetSprite*TILE_SIZE, 5*TILE_SIZE, TILE_SIZE, TILE_SIZE));
                this->gridSprite.setPosition(coord.x * (TILE_SIZE + GAP_SIZE), coord.y * (TILE_SIZE + GAP_SIZE));
                window->draw(this->gridSprite);
            }
        }
    }

    // Last: Draw the tile-highlighting cursor
    if (cursorTile.x != -1) {  // If we found a valid tile to highlight
        this->gridSprite.setTextureRect(sf::Rect<int>(0, 3*TILE_SIZE, TILE_SIZE, TILE_SIZE));
        this->gridSprite.setPosition(sf::Vector2<float>(cursorTile.x*TILE_SIZE + cursorTile.x*GAP_SIZE, cursorTile.y*TILE_SIZE + cursorTile.y*GAP_SIZE));
        window->draw(gridSprite);
    }
}

void DataBattle::setHUD(InputBox* hud) {
    this->hud = hud;
}

void DataBattle::setPlayer(Player* player) {
    this->player = player;
}

string DataBattle::play(sf::RenderWindow* window) {
    cout << "Called DataBattle::play()\n";
    sf::Texture viewSurf;
    //this->gridSheet.loadFromFile("Data\\Sprites\\Grid.png");
    this->gridSprite = sf::Sprite(GRID_SHEET);
    this->programSprite = sf::Sprite(PROGRAM_SHEET);
    this->phase = 'u'; // Upload
    this->selectedUpload = -1;

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

    while (window->isOpen()) {
        //cout << "In main loop\n";
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

        hud->setMousePos(mousePos);

        sf::Event event;
        while (window->pollEvent(event)) {
            hud->takeInput(event, this);
            if (event.type == sf::Event::Closed)
                window->close();

            if (event.type == sf::Event::KeyPressed) {
                cout << fps << '\n';
                //cout << "Victory status: " << this->checkForVictory() << '\n';
                cout << "Current program: " << this->currentProgram->name << '\n';
                for (ProgramSector* s : this->currentProgram->sectors) {
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

        // Player's turn
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

                    if (this->grid[tileCoord.x][tileCoord.y] != 0) {
                        if (tileCoord == nButton) {
                            this->currentProgram->move(nButton, false);
                            hud->setFocus(this->currentProgram);
                        } else if (tileCoord == sButton) {
                            this->currentProgram->move(sButton, false);
                            hud->setFocus(this->currentProgram);
                        } else if (tileCoord == eButton) {
                            this->currentProgram->move(eButton, false);
                            hud->setFocus(this->currentProgram);
                        } else if (tileCoord == wButton) {
                            this->currentProgram->move(wButton, false);
                            hud->setFocus(this->currentProgram);
                        }
                    }

                    this->programHead = this->currentProgram->sectors[0]->coord;
                    if (this->currentProgram->currentMove < this->currentProgram->speed) {
                        this->moveArea = getRadius(this->currentProgram->speed - this->currentProgram->currentMove, this->programHead, false);

                        this->nButton = sf::Vector2<int>(programHead.x, programHead.y - 1);
                        this->sButton = sf::Vector2<int>(programHead.x, programHead.y + 1);
                        this->eButton = sf::Vector2<int>(programHead.x + 1, programHead.y);
                        this->wButton = sf::Vector2<int>(programHead.x - 1, programHead.y);
                    } else {
                        this->targets.clear();
                        this->currentProgram->switchToAiming(0);
                        this->aimArea = this->currentProgram->currentAction->getAimArea(this->programHead, this->targets.size());
                        this->hud->setSubFocus(0);
                    }

                } else if (this->currentProgram->state == 'a') {  // If aiming
                    for (sf::Vector2i coord : this->aimArea) {
                        if (coord == tileCoord) {
                            this->targets.push_back(tileCoord);  // Add it to the target list
                            this->aimArea = this->currentProgram->currentAction->getAimArea(this->programHead, this->targets.size());
                            if (this->targets.size() >= this->currentProgram->currentAction->numOfTargets) {  // If we've selected all our targets
                                this->currentProgram->useAction(this, this->currentProgram->currentActionIndex, this->targets);
                                // Switch to next available program
                                this->switchPrograms(hud);
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
                        this->currentProgram->move(sf::Vector2<int>(programHead.x, programHead.y-1), false);
                        this->programHead = this->currentProgram->sectors[0]->coord;
                        this->moveArea = getRadius(this->currentProgram->speed, this->programHead);
                    } else if ((yDistance > 0) && ((startsWith(this->lookAt(programHead.x, programHead.y+1), "tile")) || (startsWith(this->lookAt(programHead.x, programHead.y+1), "defender " + this->currentDefenderIndex)))) {
                        // South
                        this->currentProgram->move(sf::Vector2<int>(programHead.x, programHead.y+1), false);
                        this->programHead = this->currentProgram->sectors[0]->coord;
                        this->moveArea = getRadius(this->currentProgram->speed, this->programHead);
                    } else if ((xDistance > 0) && ((startsWith(this->lookAt(programHead.x+1, programHead.y), "tile")) || (startsWith(this->lookAt(programHead.x+1, programHead.y), "defender " + this->currentDefenderIndex)))) {
                        // East
                        this->currentProgram->move(sf::Vector2<int>(programHead.x+1, programHead.y), false);
                        this->programHead = this->currentProgram->sectors[0]->coord;
                        this->moveArea = getRadius(this->currentProgram->speed, this->programHead);
                    } else if ((xDistance < 0) && ((startsWith(this->lookAt(programHead.x-1, programHead.y), "tile")) || (startsWith(this->lookAt(programHead.x-1, programHead.y), "defender " + this->currentDefenderIndex)))) {
                        // West
                        this->currentProgram->move(sf::Vector2<int>(programHead.x-1, programHead.y), false);
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
        }

        // Check to see if anybody won, and if so, end the DataBattle
        if ((this->phase != 'u') && (this->checkForVictory() != '0')) {
            if (this->checkForVictory() == 'p') {  // If the player won
                cout << "Databattle won\n";
                musicTrack.stop();
                return "";
            } else {  // If the computer won
                cout << "Databattle lost\n";
                musicTrack.stop();
                return "";
            }
        }

        //cout << "Calling render()\n";
        this->render(window);
        //cout << "Render finished\nCalling hud->render()\n";
        hud->render(window, this);
        window->display();
        //cout << "Finished main loop\n";
    }

    return "";
}

void DataBattle::switchTurns(InputBox* hud) {
    // Right now, we're crashing inside this function when programs die.  Find out what's wrong.
    cout << "Switching turns\n";
    // Check to see if any programs died during the last turn, and if so, delete them.
    bool programsDied = true;
    while (programsDied) {
        //cout << "Looking for dead friendlies\n";
        programsDied = false;
        for (int i=0; i<this->friendlies.size(); i++) {
            //cout << "Friendly " << i << '\n';
            //cout << this->friendlies[i]->state << '\n';
            if (this->friendlies[i]->state == 'x') {  // If the program is marked as dead
                cout << "Deleting dead friendly\n";
                delete this->friendlies[i];
                this->friendlies.erase(this->friendlies.begin() + i);
                programsDied = true;
                break;
            }
        }
    }
    programsDied = true;
    while (programsDied) {
        programsDied = false;
        for (pair<string, DataBattlePiece*> p : this->defenders) {
            if (this->defenders[p.first]->state == 'x') {
                delete this->defenders[p.first];
                this->defenders.erase(p.first);
                programsDied = true;
                break;
            }
        }
    }

    // Switch turns
    if (this->phase == 'u' || this->phase == 'e') { // Player or enemy
        //cout << "Switching to player\n";
        // Switch to player
        this->phase = 'p';
        // Add the textbox here
        for (DataBattlePiece* p : this->friendlies) {
            p->prepForTurn();
        }
        this->currentProgramIndex = 0;
        this->currentProgram = this->friendlies[this->currentProgramIndex];
        if (this->programStartingState != nullptr) {
            delete this->programStartingState;  // Deallocating memory
        }
        this->programStartingState = new Program(currentProgram);  // Copy the program to use as an undo point
        this->programHead = this->currentProgram->sectors[0]->coord;
        this->nButton = sf::Vector2<int>(programHead.x, programHead.y - 1);
        this->sButton = sf::Vector2<int>(programHead.x, programHead.y + 1);
        this->eButton = sf::Vector2<int>(programHead.x + 1, programHead.y);
        this->wButton = sf::Vector2<int>(programHead.x - 1, programHead.y);
        this->moveArea = getRadius(this->currentProgram->speed, this->programHead);
        hud->setFocus(this->currentProgram);

    } else {
        //cout << "Switching to enemy\n";
        // Switch to enemy
        this->phase = 'e';  // Switch to enemy
        // Add textbox here;
        bool first = true;
        for (pair<string, DataBattlePiece*> p : this->defenders) {
            if (first) {
                this->currentDefenderIndex = p.first;
                first = false;
            }
            p.second->prepForTurn();
        }
        if (!first) {  // If we found a defender to use, first will be false.  If there are no defenders, first will never have the opportunity to be set to false, and thus be true.  So we should only execute this code if first == false, because we know we found something to switch to.
            this->currentProgram = this->defenders[this->currentDefenderIndex];
            this->moveArea = getRadius(this->currentProgram->speed, this->currentProgram->sectors[0]->coord);
        }
    }
    //cout << "Done switching turns\n";
}

void DataBattle::switchPrograms(InputBox* hud) {  // Find the next available program and switch to it
    cout << "Switching programs\n";
    bool outOfPrograms = true;
    if (this->currentProgram->invisibilityTimer > 0) {  // If the program's temporarily invisible
        cout << "Decrementing invisibility\n";
        this->currentProgram->invisibilityTimer--;  // Knock its invisibility down a notch
        if (this->currentProgram->invisibilityTimer == 0) {  // If it's run out of invisibility
            this->currentProgram->visible = true;
        }
    }
    if (this->phase == 'e') { // Enemy turn
        for (pair<string, DataBattlePiece*> p : this->defenders) {
            if (p.second->state != 'd' && p.second->state != 'x') {
                this->currentDefenderIndex = p.first;
                this->currentProgram = this->defenders[this->currentDefenderIndex];
                this->moveArea = getRadius(this->currentProgram->speed, this->currentProgram->sectors[0]->coord);
                outOfPrograms = false;
                break;
            }
        }
    } else if (this->phase == 'p') { // Player turn
        for (int i=0; i<this->friendlies.size(); i++) {
            if (this->friendlies[i]->state != 'd' && this->friendlies[i]->state != 'x') {
                this->currentProgramIndex = i;
                this->currentProgram = this->friendlies[i];
                cout << "Copying program starting state\n";
                delete this->programStartingState;  // Deallocating memory
                this->programStartingState = new Program(this->currentProgram);  // Copy the program to use as an undo point
                this->programHead = this->currentProgram->sectors[0]->coord;
                this->nButton = sf::Vector2<int>(programHead.x, programHead.y - 1);
                this->sButton = sf::Vector2<int>(programHead.x, programHead.y + 1);
                this->eButton = sf::Vector2<int>(programHead.x + 1, programHead.y);
                this->wButton = sf::Vector2<int>(programHead.x - 1, programHead.y);
                this->moveArea = getRadius(this->currentProgram->speed, this->programHead);
                hud->setFocus(this->currentProgram);
                outOfPrograms = false;
                break;
            }
        }
    }
    if (outOfPrograms) {
        this->switchTurns(hud);
    }
}

string DataBattle::takeCommand(string command) {
    if (startsWith(command, "upload")) {
        vector<string> splitCommand = splitString(command, ' ');
        string programName = splitCommand[1];
        // See if there's a friendly program there
        for (int i=0; i < this->friendlies.size(); i++) {
            for (ProgramSector* s : this->friendlies[i]->sectors) {
                if (s->coord == this->uploads[stoi(splitCommand[2])]) {
                    return "conflict";
                }
            }
        }
    }
    if (startsWith(command, "look")) {
        vector<string> splitCommand = splitString(command, ' ');
        sf::Vector2<int> coord = sf::Vector2<int>(stoi(splitCommand[1]), stoi(splitCommand[2]));
        return this->lookAt(coord);
    }

    return "";
}

string DataBattle::lookAt(sf::Vector2i coord) {
    // Uploads
    if (this->phase == 'u') {
        for (int i=0; i<this->uploads.size(); i++) {
            if (coord == this->uploads[i]) {
                this->selectedUpload = i;
                return "upload " + to_string(i);
            }
        }
    }
    // Cash pickups
    for (sf::Vector2i c : this->cashPickups) {
        if (c == coord) {
            return "cash";
        }
    }
    // Defenders
    for (pair<string, DataBattlePiece*> p : this->defenders) {
        for (ProgramSector* s : p.second->sectors) {
            if (s->coord == coord) {
                return "defender " + p.first;
            }
        }
    }

    // Friendlies
    for (int i=0; i<this->friendlies.size(); i++) {
        for (ProgramSector* s : this->friendlies[i]->sectors) {
            if (s->coord == coord) {
                return "friendly " + to_string(i);
            }
        }
    }

    // Tiles
    if (this->grid[coord.x][coord.y] == 0) {
        return "empty";
    } else {
        return "tile";
    }
}

string DataBattle::lookAt(int x, int y) {
    sf::Vector2<int> coord(x, y);
    return this->lookAt(coord);
}

char DataBattle::checkForVictory() {
    // Check friendlies
    bool playerStillAlive = false;
    for (int i=0; i<this->friendlies.size(); i++) {
        if (this->friendlies[i]->state != 'x') {
            playerStillAlive = true;
        }
    }

    // Check defenders
    bool compStillAlive = false;
    for (pair<string, DataBattlePiece*> p : this->defenders) {
        if (p.second->state != 'x') {
            compStillAlive = true;
        }
    }
    if (compStillAlive && playerStillAlive) {
        return '0'; // No one's won yet
    } else if (compStillAlive && !playerStillAlive) {
        return 'c'; // The computer won
    } else if (playerStillAlive && !compStillAlive) {
        return 'p'; // The player won
    }
    // Assuming you reached this point, both player and computer are dead
    return 'd'; // Draw
}

void DataBattle::flipSector(sf::Vector2i coord) {
    if (this->grid[coord.x][coord.y] == 0) {
        this->grid[coord.x][coord.y] = 1;
    } else {
        this->grid[coord.x][coord.y] = 0;
    }
}
