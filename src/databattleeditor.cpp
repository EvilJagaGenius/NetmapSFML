#include "databattleeditor.h"

DataBattleEditor::DataBattleEditor(string filename) {
    this->db = nullptr;
    this->gridSprite = sf::Sprite(GRID_SHEET);
    this->programSprite = sf::Sprite(PROGRAM_SHEET);
    this->textBox = sf::Text("", DEFAULT_FONT, 12);
    this->button = sf::Rect<int>(WY, 0, 100, 14);
    this->buttonGraphic = sf::RectangleShape(sf::Vector2<float>(100, 14));
    this->buttonGraphic.setFillColor(sf::Color::Transparent);
    this->buttonGraphic.setOutlineColor(sf::Color::White);
    this->buttonGraphic.setOutlineThickness(1);
    this->currentInputBox = nullptr;
    this->destination = "title:";

    if (filename.size() > 0) {  // If it's not an empty string
        this->loadDB(filename);
    }
}

DataBattleEditor::DataBattleEditor() {
    this->db = nullptr;
    this->gridSprite = sf::Sprite(GRID_SHEET);
    this->programSprite = sf::Sprite(PROGRAM_SHEET);
    this->textBox = sf::Text("", DEFAULT_FONT, 12);
    this->button = sf::Rect<int>(WY, 0, 100, 14);
    this->buttonGraphic = sf::RectangleShape(sf::Vector2<float>(100, 14));
    this->buttonGraphic.setFillColor(sf::Color::Transparent);
    this->buttonGraphic.setOutlineColor(sf::Color::White);
    this->buttonGraphic.setOutlineThickness(1);
    this->currentInputBox = nullptr;
    this->destination = "title:";
}

DataBattleEditor::~DataBattleEditor() {
    if (this->db != nullptr) {
        delete this->db;
    }
}

void DataBattleEditor::newDB(string filename) {
    if (!endsWith(filename, ".txt")) {
        filename += ".txt";
    }

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
    this->programCounter = 0;
}

void DataBattleEditor::loadDB(string filename) {
    if (this->db != nullptr) {
        delete this->db;
    }
    if (endsWith(filename, ".txt")) {
        this->db = new DataBattle(filename.substr(0, filename.size()-4));
    } else {
        this->db = new DataBattle(filename);
    }
    this->programCounter = this->db->defenders.size();
}

void DataBattleEditor::saveDB(string filename) {
    if (!endsWith(filename, ".txt")) {
        filename += ".txt";
    }

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
            for (int i=1; i<p->sectors.size(); i++) {  // For every sector in the program
                sf::Vector2i coord = p->sectors[i]->coord;
                //cout << "Coord i = " << getByteCoord(coord) << '\n';
                // Find a sector it's connected to
                bool foundConnection = false;
                for (int j=0; j<p->sectors.size(); j++) {  // Loop through every sector in the body
                    //sf::Vector2 coordj = p->sectors[j]->coord;
                    //cout << "Coord j = " << getByteCoord(coordj) << '\n';
                    for (ProgramSector* l : p->sectors[j]->links) {
                        if (l->coord == coord) {
                            cout << "Saving added sector\n";
                            foundConnection = true;
                            newFile << "addSector:" << element.first << ':' << to_string(coord.x) << ':' << to_string(coord.y) << ':' << to_string(j) << '\n';
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
    //cout << "Rendering\n";

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
    // Draw new/load buttons
    this->button.top = 0;
    this->textBox.setColor(sf::Color::White);
    this->textBox.setPosition(WY, this->button.top);
    this->textBox.setString("Exit Editor");
    window->draw(this->textBox);
    if (this->button.contains(this->mousePos)) {
        //cout << "Drawing button graphic\n";
        this->buttonGraphic.setPosition(this->button.left, this->button.top);
        window->draw(this->buttonGraphic);
    }

    this->button.top = 14;
    this->textBox.setPosition(WY, this->button.top);
    this->textBox.setString("New DataBattle");
    window->draw(this->textBox);
    if (this->button.contains(this->mousePos)) {
        this->buttonGraphic.setPosition(this->button.left, this->button.top);
        window->draw(this->buttonGraphic);
    }

    this->button.top = 28;
    this->textBox.setPosition(WY, this->button.top);
    this->textBox.setString("Load DataBattle");
    window->draw(this->textBox);
    if (this->button.contains(this->mousePos)) {
        this->buttonGraphic.setPosition(this->button.left, this->button.top);
        window->draw(this->buttonGraphic);
    }

    // Save DB, Play DB
    if (this->db != nullptr) {
        this->button.top = 42;
        this->textBox.setPosition(WY, this->button.top);
        this->textBox.setString("Save DataBattle");
        window->draw(this->textBox);
        if (this->button.contains(this->mousePos)) {
            this->buttonGraphic.setPosition(this->button.left, this->button.top);
            window->draw(this->buttonGraphic);
        }

        this->button.top = 56;
        this->textBox.setPosition(WY, this->button.top);
        this->textBox.setString("Save as:");
        window->draw(this->textBox);
        if (this->button.contains(this->mousePos)) {
            this->buttonGraphic.setPosition(this->button.left, this->button.top);
            window->draw(this->buttonGraphic);
        }

        this->button.top = 70;
        this->textBox.setPosition(WY, this->button.top);
        this->textBox.setString("Play DataBattle");
        window->draw(this->textBox);
        if (this->button.contains(this->mousePos)) {
            this->buttonGraphic.setPosition(this->button.left, this->button.top);
            window->draw(this->buttonGraphic);
        }
    }

    // Last: Draw input boxes
    if (this->currentInputBox != nullptr) {
        this->currentInputBox->render(window, this);
    }
}

void DataBattleEditor::clearTile(sf::Vector2i coord) {
    // Look through uploads
    for (int i=0; i<this->db->uploads.size(); i++) {
        if (this->db->uploads[i] == coord) {
            this->db->uploads.erase(this->db->uploads.begin()+i);
            break;
        }
    }
    // Look through defenders
    for (pair<string, DataBattlePiece*> element : this->db->defenders) {
        DataBattlePiece* p = element.second;
        for (ProgramSector* s : p->sectors) {
            if (s->coord == coord) {
                p->amputate(coord);
                break;
            }
        }
    }
    // See if we killed anything by amputation
    bool programsDied = true;
    while (programsDied) {
        programsDied = false;
        for (pair<string, DataBattlePiece*> element : this->db->defenders) {
            if (element.second->state == 'x') {
                delete element.second;
                this->db->defenders.erase(element.first);
                programsDied = true;
                break;
            }
        }
    }
    // Add more here when the time comes
}

string DataBattleEditor::play(sf::RenderWindow* window) {
    cout << "Called DataBattleEditor::play()\n";
    bool rightClicked = false;
    bool leftClicked = false;
    this->currentInputBox = nullptr;
    sf::Vector2i tileCoord;
    sf::Vector2i selectedCoord;

    DataBattlePiece* piece = nullptr;
    int sectorIndex = -1;

    // Main loop
    while (window->isOpen()) {
        window->clear();
        this->mousePos = sf::Mouse::getPosition(*window);
        leftClicked = false;
        rightClicked = false;
        tileCoord = sf::Vector2<int>(-1, -1);
        if (mousePos.x <= WY) {
            if ((mousePos.x % (TILE_SIZE + GAP_SIZE) <= TILE_SIZE) && (mousePos.y % (TILE_SIZE + GAP_SIZE) <= TILE_SIZE)) {
                tileCoord = sf::Vector2<int>(mousePos.x/(TILE_SIZE + GAP_SIZE), mousePos.y/(TILE_SIZE + GAP_SIZE));
            }
        }

        sf::Event event;
        while (window->pollEvent(event)) {
            // Give input to the input boxes we have open
            if (this->currentInputBox != nullptr) {
                this->currentInputBox->setMousePos(this->mousePos);
                this->currentInputBox->takeInput(event, this);
            }

            if (event.type == sf::Event::Closed) {
                window->close();
            } else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    leftClicked = true;
                } else if (event.mouseButton.button == sf::Mouse::Right) {
                    rightClicked = true;
                }
            } else if (event.type == sf::Event::KeyPressed) {
                if ((event.key.code == sf::Keyboard::S) && (event.key.control)) {  // Ctrl-S
                    this->saveDB(this->db->filename);
                }
            }
        }
        // See if we clicked any side-panel buttons
        if (leftClicked) {
            // Exit Editor
            this->button.top = 0;
            if (this->button.contains(this->mousePos)) {
                return this->destination;
            }

            // New DB
            this->button.top = 14;
            if (this->button.contains(this->mousePos)) {
                if (this->currentInputBox != nullptr) {
                    delete this->currentInputBox;
                }
                this->currentInputBox = new TextInputBox("Enter the name of the new DataBattle:");
                this->inputBoxType = 'n'; // n for New DB
            }

            // Load DB
            this->button.top = 28;
            if (this->button.contains(this->mousePos)) {
                if (this->currentInputBox != nullptr) {
                    delete this->currentInputBox;
                }
                this->currentInputBox = new TextInputBox("Enter the name of the DataBattle to load:");
                this->inputBoxType = 'l';
            }

            if (this->db != nullptr) { // Save DB
                this->button.top = 42;
                if (this->button.contains(this->mousePos)) {
                    this->saveDB(this->db->filename);
                }

                this->button.top = 56;  // Save as
                if (this->button.contains(this->mousePos)) {
                    if (this->currentInputBox != nullptr) {
                        delete this->currentInputBox;
                    }
                    this->currentInputBox = new TextInputBox("Save DataBattle as:");
                    this->inputBoxType = 'a';
                }

                this->button.top = 70;  // Play DB
                if (this->button.contains(this->mousePos)) {
                    cout << "Play DataBattle\n";
                    // We could do this->db->play().  But that'd be running a playable inside another playable, and I'd rather _not_ do that.
                    // Add a loop in main.cpp to take care of switching playables.
                    this->saveDB(this->db->filename);  // This won't work right if we don't save our changes first... we could save into a temp file, maybe?
                    return ("dbFromEditor:" + this->db->filename);
                }
            }

            if (tileCoord.x != -1 && this->currentInputBox == nullptr) {  // If we're pointing to a tile on the grid (and there's no dialog box up)
                //cout << "Valid tile\n";
                if (startsWith(this->db->lookAt(tileCoord), "tile")) {  // If that spot's an empty tile
                    this->db->grid[tileCoord.x][tileCoord.y] = 0;
                } else if (startsWith(this->db->lookAt(tileCoord), "empty")) {  // If it's empty
                    this->db->grid[tileCoord.x][tileCoord.y] = 1;
                }
                //cout << this->db->lookAt(tileCoord) << '\n';
                //cout << this->db->grid[tileCoord.x][tileCoord.y] << '\n';
            }
        }

        if (rightClicked) {
            //cout << "Right-clicked\n";
            if (tileCoord.x != -1) { // If we're on a valid tile
                selectedCoord = sf::Vector2<int>(tileCoord);
                if (this->currentInputBox != nullptr) {
                    delete this->currentInputBox;
                }
                vector<string> options = {"Close Box", "Clear Tile", "New Program", "Add Sector", "Add Upload"};
                vector<bool> usable = {true, false, false, false, false};
                if (this->db != nullptr && selectedCoord.x != -1) {
                    if (!startsWith(this->db->lookAt(selectedCoord), "empty")) {  // Clear tile is usable
                        usable[1] = true;
                    }
                    if (startsWith(this->db->lookAt(selectedCoord), "tile")) {  // New Program and Add Upload are usable
                        usable[2] = true;
                        usable[4] = true;
                    }
                    if (startsWith(this->db->lookAt(selectedCoord), "defender")) {  // Add sector is usable
                        usable[3] = true;
                        piece = this->db->defenders[splitString(this->db->lookAt(selectedCoord), ' ')[1]];
                        for (int i=0; i<piece->size; i++) {
                            ProgramSector* s = piece->sectors[i];
                            if (s->coord == selectedCoord) {
                                sectorIndex = i;
                                break;
                            }
                        }
                    }
                }

                this->currentInputBox = new ChoiceInputBox(mousePos, options, usable, 10);
                this->inputBoxType = 'c';
            }
        }

        if (this->currentInputBox != nullptr) {
            if (this->currentInputBox->done) {
                if (this->inputBoxType == 'n') {  // If it was the New DB dialog:
                    string filename = this->currentInputBox->getFocus();
                    if (filename.size() > 0) {
                        this->newDB(filename);
                    }
                    delete this->currentInputBox;
                    this->currentInputBox = nullptr;
                    this->inputBoxType = '0';
                } else if (this->inputBoxType == 'l') {  // Load DB
                    string filename = this->currentInputBox->getFocus();
                    if (filename.size() > 0) {
                        this->loadDB(filename);
                    }
                    delete this->currentInputBox;
                    this->currentInputBox = nullptr;
                    this->inputBoxType = '0';
                } else if (this->inputBoxType == 'c') { // Choice box
                    int option = this->currentInputBox->getSubFocus();
                    delete this->currentInputBox;
                    this->currentInputBox = nullptr;
                    this->inputBoxType = '0';

                    if (option == 1) {  // Clear tile
                        cout << "Clear tile\n";
                        this->clearTile(selectedCoord);
                        cout << "Done clearing tile\n";
                    } else if (option == 2) {  // New Program
                        // Bring up another choice box to select the program
                        vector<string> options;
                        vector<bool> usable;
                        for (pair<string, Program*> element : PROGRAM_DB) {
                            options.push_back(element.first);
                            usable.push_back(true);
                        }
                        this->currentInputBox = new ChoiceInputBox(mousePos, options, usable, 10);
                        this->inputBoxType = 'p';
                    } else if (option == 3) {  // Add sector
                        this->currentInputBox = new SectorInputBox();
                        this->inputBoxType = 's';
                        cout << "Add sector\n";
                    } else if (option == 4) {  // Add upload
                        if (startsWith(this->db->lookAt(selectedCoord), "tile")) {
                            this->db->uploads.push_back(selectedCoord);
                        }
                    }

                } else if (this->inputBoxType == 'p') {  // New Program
                    string programType = this->currentInputBox->getFocus();
                    delete this->currentInputBox;
                    this->currentInputBox = nullptr;
                    this->inputBoxType = '0';

                    //cout << "Add " << programType << " at " << getByteCoord(selectedCoord) << '\n';
                    // We have the data we need to create a new program.  Let's do it
                    Program* newProgram = new Program(programType);
                    newProgram->move(selectedCoord, true);
                    newProgram->owner = 'c';
                    this->db->defenders.insert({{"defender" + to_string(this->programCounter), newProgram}});
                    this->programCounter++;
                } else if (this->inputBoxType == 's') {  // Add sector
                    // When we hit this point, tileCoord is where we want to create our new sector
                    delete this->currentInputBox;
                    this->currentInputBox = nullptr;
                    this->inputBoxType = '0';
                    if (tileCoord.x != -1) {  // Check for valid coord
                        if (startsWith(this->db->lookAt(tileCoord), "tile")) {
                            //ProgramSector* newSector = new ProgramSector(tileCoord);
                            piece->addSector(tileCoord, sectorIndex);
                            piece = nullptr;
                            sectorIndex = -1;
                        }
                    }
                } else if (this->inputBoxType == 'a') {  // Save as
                    string filename = this->currentInputBox->getFocus();
                    if (filename.size() > 0) {
                        this->saveDB(filename);
                    }
                    delete this->currentInputBox;
                    this->currentInputBox = nullptr;
                    this->inputBoxType = '0';
                }
            }
        }

        this->render(window);
        window->display();
    }
    return "quit:";
}
