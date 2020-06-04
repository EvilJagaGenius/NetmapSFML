#include "databattle.h"

DataBattle::DataBattle() {

}

DataBattle::DataBattle(string filename) {
    this->filename = filename;
    this->programStartingState = nullptr;
    this->pieces.clear();

    for (int i=0; i<16; i++) {
        for (int j=0; j<16; j++) {
            this->grid[i][j] = 0;
        }
    }

    this->load();
}

DataBattle::~DataBattle() {
    // Remember to fill this in, deallocate your programs.
    for (int i=0; i<this->pieces.size(); i++) {
        delete this->pieces[i];
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
                    UploadZone* newUpload = new UploadZone(stoi(splitLine[1]), stoi(splitLine[2]), stoi(splitLine[3]));
                    this->pieces.push_back(newUpload);
                }

                if (startsWith(line, "addProgram")) {
                    // 1:DataBattlePiece type, 2:x, 3:y, 4:owner, 5:name
                    cout << "Adding defender\n";
                    vector<string> splitLine = splitString(line, ':');
                    Program* newProgram = new Program(splitLine[1]);
                    newProgram->move(sf::Vector2<int>(stoi(splitLine[2]), stoi(splitLine[3])), true);
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



void DataBattle::switchTurns() {
    // If necessary, insert code to delete dead pieces
    // We should add a victory check before going into the loop
    bool foundFirstProgram = false;
    while (!foundFirstProgram) {  // This could go infinite
        this->currentPlayerIndex = ++this->currentPlayerIndex % this->players.size();
        // Find a first program
        for (int i=0; i<this->pieces.size(); i++) {
            DataBattlePiece* piece = this->pieces[i];
            if (piece->controller == currentPlayerIndex) {
                piece->prepForTurn();
                if (piece->pieceType == 'p') {  // Or avatar or user, etc
                    if (!foundFirstProgram) {
                        foundFirstProgram = true;
                        this->nextProgram = piece;
                        this->nextProgramIndex = i;
                    }
                }
            }
        }
    }
    this->switchPrograms();
    //cout << "Done switching turns\n";
}

void DataBattle::switchPrograms() {  // Find the next available program and switch to it
    cout << "Switching programs\n";
    this->currentProgram->tickStatuses();  // Maybe we want to call this someplace else

    if (this->nextProgram == nullptr) {
        this->switchTurns();
    } else {
        // Switch focus to nextProgram
        this->currentProgram = this->nextProgram;
        this->currentProgramIndex = this->nextProgramIndex;
        // Search until you find another of the player's programs, ready to use
        for (int i=0; i<this->pieces.size(); i++) {
            int index = i % this->pieces.size();
            DataBattlePiece* piece = this->pieces[index];
            if (piece->pieceType == 'p') {  // Or avatar or user, etc
                // Set nextProgram to that, maybe add a 'next' marker to it
                this->nextProgram = piece;
                this->nextProgramIndex = index;
            }
        }
        // If you can't find another one of those programs, nextProgram = nullptr
        if (this->currentProgramIndex == this->nextProgramIndex) {
            nextProgram = nullptr;
            nextProgramIndex = -1;
        }
    }
}

string DataBattle::takeCommand(string command) {
    /*if (startsWith(command, "upload")) {
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
    }*/

    return "Not implemented";
}

string DataBattle::lookAt(sf::Vector2i coord) {
    /*// Uploads
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
    }*/
    return "Not implemented";
}

string DataBattle::lookAt(int x, int y) {
    sf::Vector2<int> coord(x, y);
    return this->lookAt(coord);
}

int DataBattle::checkForVictory() {
    /*// Check friendlies
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
    return 'd'; // Draw*/
    return '0';
}

void DataBattle::flipSector(sf::Vector2i coord) {
    if (this->grid[coord.x][coord.y] == 0) {
        this->grid[coord.x][coord.y] = 1;
    } else {
        this->grid[coord.x][coord.y] = 0;
    }
}
