#include "databattle.h"

DataBattle::DataBattle() {

}

DataBattle::DataBattle(string filename) {
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
    cout << "Loading DB\n";
    ifstream textFile;
    textFile.open("Data\\DataBattles\\" + filename + ".txt");
    string line;
    char loading = '0';  // 0 if we're not loading anything

    int gridY = 0;

    this->destination = "title:";

    while(getline(textFile, line)) {
        if (startsWith(line, "bkg:")) {  // Background
            this->bkgFilename = line.substr(4);
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
                    cout << "Adding upload zone\n";
                    vector<string> splitLine = splitString(line, ':');
                    UploadZone newUpload = new UploadZone(stoi(splitLine[1]), stoi(splitLine[2]), stoi(splitLine[3]));
                    this->pieces.push_back(newUpload);
                }

                if (startsWith(line, "addProgram")) {
                    // 1:DataBattlePiece type, 2:x, 3:y, 4:owner, 5:name
                    cout << "Adding defender\n";
                    vector<string> splitLine = splitString(line, ':');
                    Program* newProgram = new Program(splitLine[1]);
                    newProgram->move(this, sf::Vector2<int>(stoi(splitLine[2]), stoi(splitLine[3])), true);
                    newProgram->owner = stoi(splitLine[4]);
                    newProgram->name = splitLine[5];
                    this->pieces.push_back(newProgram);
                }

                // We need a better way to search for pieces before reimplementing this
                /*if (startsWith(line, "addSector")) {
                    cout << "Adding sector\n";
                    vector<string> splitLine = splitString(line, ':');
                    DataBattlePiece* targetProgram = this->defenders[splitLine[1]];
                    cout << "Got target piece\n";
                    int targetSector = stoi(splitLine[4]);
                    cout << "Got target sector\n";
                    targetProgram->addSector(sf::Vector2<int>(stoi(splitLine[2]), stoi(splitLine[3])), targetSector);
                }*/
            }
        }
    }
    textFile.close();
    cout << "Done loading DB\n";
}



void DataBattle::switchTurns(InputBox* hud) {
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

void DataBattle::switchPrograms() {  // Find the next available program and switch to it
    // Notes to self
    // - Rewrite this
    // - Fix the invisibility code so that it works for all status conditions
    // - Add a marker to show what program we're switching to after this
    cout << "Switching programs\n";
    bool outOfPrograms = true;
    this->currentProgram->tickStatuses();  // Maybe we want to call this someplace else
    /*if (this->phase == 'e') { // Enemy turn
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
    }*/
    if (this->nextProgram == nullptr) {
        this->switchTurns();
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
