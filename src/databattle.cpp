#include "databattle.h"

DataBattle::DataBattle() {

}

DataBattle::DataBattle(string filename)
{
    this->playableType = 'd';

    this->filename = filename;

    this->friendliesLoaded = 0;


    for (int i=0; i<16; i++) {
        for (int j=0; j<16; j++) {
            this->grid[i][j] = 0;
        }
    }

    this->load();
}

DataBattle::~DataBattle()
{
    //dtor
    // Remember to fill this in
}

void DataBattle::load() {
    ifstream textFile;
    textFile.open("Data\\DataBattles\\" + filename + ".txt");
    string line;
    char loading = '0';  // 0 if we're not loading anything

    int gridY = 0;

    while(getline(textFile, line)) {
        if (startsWith(line, "bkg:")) {  // Background
            string bkgFilename = line.substr(4);
            this->bkg = imgLoad("Data\\DataBattleBkgs\\" + bkgFilename);
            this->bkgSprite = *(new sf::Sprite(this->bkg));
        }
        if (startsWith(line, "cash:")) {  // Cash reward
            this->cashReward = stoi(line.substr(5));
        }
        if (startsWith(line, "music:")) {  // Music (nothing right now)
            // Do something, Taipu
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
                    // 1:Program type, 2:x, 3:y, 4:Reference name
                    vector<string> splitLine = splitString(line, ':');
                    Program* newProgram = new Program(splitLine[1]);
                    newProgram->move(*(new sf::Vector2<int>(stoi(splitLine[2]), stoi(splitLine[3]))), true);
                    this->defenders.insert({{splitLine[4], newProgram}});
                }

                if (startsWith(line, "addSector")) {
                    vector<string> splitLine = splitString(line, ':');
                    Program* targetProgram = this->defenders[splitLine[1]];
                    int targetSector = stoi(splitLine[4]);
                    targetProgram->addSector(*(new sf::Vector2<int>(stoi(splitLine[2]), stoi(splitLine[3]))), targetSector);
                }

            }
        }
    }
    textFile.close();
}

void DataBattle::render(sf::RenderWindow* window) {
    // Draw the background
    window->draw(this->bkgSprite);
    sf::Vector2<int> cursorTile(-1, -1);

    // Draw the grid
    for (int x=0; x<16; x++) {
        for (int y=0; y<16; y++) {
            sf::Rect<int> tileRect(x*TILE_SIZE + x*GAP_SIZE, y*TILE_SIZE + y*GAP_SIZE, TILE_SIZE, TILE_SIZE);
            this->gridSprite.setTextureRect(*(new sf::Rect<int>(0, this->grid[x][y]*TILE_SIZE, TILE_SIZE, TILE_SIZE)));
            this->gridSprite.setPosition(*(new sf::Vector2<float>(x*TILE_SIZE + x*GAP_SIZE, y*TILE_SIZE + y*GAP_SIZE)));
            window->draw(this->gridSprite);

            if (tileRect.contains(this->mousePos)) {
                cursorTile = *(new sf::Vector2<int>(x, y));
            }
        }
    }
    // Draw defenders
    for (pair<string, Program*> element : defenders) {
        Program* p = element.second;
        for (int i=0; i<(p->size); i++) {
            ProgramSector sector = p->sectors[i];
            if (i==0) {  // Blit head sprite
                this->programSprite.setTextureRect(*(new sf::Rect<int>(p->spriteCoord.x*TILE_SIZE, p->spriteCoord.y*TILE_SIZE, TILE_SIZE, TILE_SIZE)));
                this->programSprite.setPosition(*(new sf::Vector2<float>(sector.coord.x*TILE_SIZE + sector.coord.x*GAP_SIZE, sector.coord.y*TILE_SIZE + sector.coord.y*GAP_SIZE)));
                window->draw(this->programSprite);
            } else {  // Blit tail sprite
                this->programSprite.setTextureRect(*(new sf::Rect<int>(p->spriteCoord.x*TILE_SIZE, (p->spriteCoord.y + 1)*TILE_SIZE, TILE_SIZE, TILE_SIZE)));
                this->programSprite.setPosition(*(new sf::Vector2<float>(sector.coord.x*TILE_SIZE + sector.coord.x*GAP_SIZE, sector.coord.y*TILE_SIZE + sector.coord.y*GAP_SIZE)));
                window->draw(this->programSprite);
            }
        }
    }

    // Draw friendlies
    for (int i=0; i<this->friendlies.size(); i++) {
        Program* p = this->friendlies[i];
        for (int j=0; j<(p->size); j++) {
            ProgramSector sector = p->sectors[j];
            if (j==0) {  // Blit head sprite
                this->programSprite.setTextureRect(*(new sf::Rect<int>(p->spriteCoord.x*TILE_SIZE, p->spriteCoord.y*TILE_SIZE, TILE_SIZE, TILE_SIZE)));
                this->programSprite.setPosition(*(new sf::Vector2<float>(sector.coord.x*TILE_SIZE + sector.coord.x*GAP_SIZE, sector.coord.y*TILE_SIZE + sector.coord.y*GAP_SIZE)));
                window->draw(this->programSprite);
            } else {  // Blit tail sprite
                this->programSprite.setTextureRect(*(new sf::Rect<int>(p->spriteCoord.x*TILE_SIZE, (p->spriteCoord.y + 1)*TILE_SIZE, TILE_SIZE, TILE_SIZE)));
                this->programSprite.setPosition(*(new sf::Vector2<float>(sector.coord.x*TILE_SIZE + sector.coord.x*GAP_SIZE, sector.coord.y*TILE_SIZE + sector.coord.y*GAP_SIZE)));
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
                this->gridSprite.setTextureRect(*(new sf::Rect<int>(1*TILE_SIZE, 2*TILE_SIZE, TILE_SIZE, TILE_SIZE)));
            } else {
                this->gridSprite.setTextureRect(*(new sf::Rect<int>(0, 2*TILE_SIZE, TILE_SIZE, TILE_SIZE)));
            }
            window->draw(this->gridSprite);
        }
    } else {
        // Draw movement area, associated buttons
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
    }

    // Last: Draw the tile-highlighting cursor
    if (cursorTile.x != -1) {  // If we found a valid tile to highlight
        this->gridSprite.setTextureRect(*(new sf::Rect<int>(0, 3*TILE_SIZE, TILE_SIZE, TILE_SIZE)));
        this->gridSprite.setPosition(*(new sf::Vector2<float>(cursorTile.x*TILE_SIZE + cursorTile.x*GAP_SIZE, cursorTile.y*TILE_SIZE + cursorTile.y*GAP_SIZE)));
        window->draw(gridSprite);
    }
}

void DataBattle::setHUD(InputBox* hud) {
    this->hud = hud;
}

void DataBattle::setPlayer(Player* player) {
    this->player = player;
}

void DataBattle::play(sf::RenderWindow* window) {
    sf::Texture viewSurf;
    //this->gridSheet.loadFromFile("Data\\Sprites\\Grid.png");
    this->gridSprite = *(new sf::Sprite(GRID_SHEET));
    this->programSprite = *(new sf::Sprite(PROGRAM_SHEET));
    this->phase = 'u'; // Upload
    this->selectedUpload = -1;

    // FPS measurements
    sf::Clock chrono;
    float lastTime = 0;
    float currentTime;
    float fps;

    while (window->isOpen()) {
        // Do something, Taipu
        window->clear();
        this->mousePos = sf::Mouse::getPosition(*window);

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
            }
        }

        this->render(window);
        hud->render(window, this);
        window->display();
    }
}

void DataBattle::switchTurns(InputBox* hud) {
    if (this->phase == 'u' || this->phase == 'e') { // Player or enemy
        // Switch to player
        this->phase = 'p';
        // Add the textbox here
        for (Program* p : this->friendlies) {
            p->prepForTurn();
        }
        this->currentProgramIndex = 0;
        this->currentProgram = this->friendlies[this->currentProgramIndex];
        // Copy the program for undo operations here
        this->programHead = this->currentProgram->sectors[0].coord;
        this->nButton = sf::Vector2<int>(programHead.x, programHead.y - 1);
        this->sButton = sf::Vector2<int>(programHead.x, programHead.y + 1);
        this->eButton = sf::Vector2<int>(programHead.x + 1, programHead.y);
        this->wButton = sf::Vector2<int>(programHead.x - 1, programHead.y);
        this->moveArea = getRadius(this->currentProgram->speed, this->programHead);
        hud->setFocus(this->currentProgram);

    } else {
        // Switch to enemy
        this->phase = 'e';  // Switch to enemy
        // Add textbox here;
        bool first = true;
        for (pair<string, Program*> p : this->defenders) {
            if (first) {
                this->currentDefenderIndex = p.first;
                first = false;
            }
            p.second->prepForTurn();
        }
        this->currentProgram = this->defenders[this->currentDefenderIndex];
        this->moveArea = getRadius(this->currentProgram->speed, this->currentProgram->sectors[0].coord);
    }
}

string DataBattle::takeCommand(string command) {
    if (startsWith(command, "upload")) {
        vector<string> splitCommand = splitString(command, ' ');
        string programName = splitCommand[1];
        // See if there's a friendly program there
        for (int i=0; i < this->friendlies.size(); i++) {
            for (ProgramSector s : this->friendlies[i]->sectors) {
                if (s.coord == this->uploads[stoi(splitCommand[2])]) {
                    return "conflict";
                }
            }
        }
    }
    if (startsWith(command, "look")) {
        vector<string> splitCommand = splitString(command, ' ');
        sf::Vector2<int> coord = *(new sf::Vector2<int>(stoi(splitCommand[1]), stoi(splitCommand[2])));
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
    for (pair<string, Program*> p : this->defenders) {
        for (ProgramSector s : p.second->sectors) {
            if (s.coord == coord) {
                return "defender " + p.first;
            }
        }
    }

    // Friendlies
    for (int i=0; i<this->friendlies.size(); i++) {
        for (ProgramSector s : this->friendlies[i]->sectors) {
            if (s.coord == coord) {
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
