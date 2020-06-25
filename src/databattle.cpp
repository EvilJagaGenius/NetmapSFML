#include "databattle.h"

DataBattle::DataBattle() {

}

DataBattle::DataBattle(string filename) {
    this->pieceCounter = 0;
    this->filename = filename;
    this->programStartingState = nullptr;
    this->pieces.clear();
    this->currentPlayerIndex = -1;

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
                    this->addPiece(newUpload);
                }

                if (startsWith(line, "addProgram")) {
                    // 1:DataBattlePiece type, 2:x, 3:y, 4:owner, 5:name
                    cout << "Adding defender\n";
                    vector<string> splitLine = splitString(line, ':');
                    Program* newProgram = new Program(splitLine[1]);
                    newProgram->move(sf::Vector2<int>(stoi(splitLine[2]), stoi(splitLine[3])), true);
                    newProgram->owner = stoi(splitLine[4]);
                    newProgram->name = splitLine[5];
                    this->addPiece(newProgram);
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

void DataBattle::addPlayer(Player* player) {
    this->players.push_back(player);
}


void DataBattle::switchTurns() {
    // If necessary, insert code to delete dead pieces
    // We should add a victory check before going into the loop
    cout << "Switching turns\n";

    if (this->currentPlayerIndex == -1) {  // If we're starting from the upload phase
        // Delete all upload zones
        cout << "Deleting upload zones\n";

        bool killedAllUploads = false;
        DataBattlePiece* uploadZone = nullptr;
        int uploadIndex = -1;
        while (killedAllUploads == false) {
            killedAllUploads = true;
            uploadZone = nullptr;
            uploadIndex = -1;
            for (int i=0; i<this->pieces.size(); i++) {
                if (this->pieces[i]->pieceType == 'u') {
                    uploadZone = this->pieces[i];
                    uploadIndex = i;
                    killedAllUploads = false;
                    break;
                }
            }
            if (uploadZone != nullptr) {
                this->pieces.erase(this->pieces.begin() + uploadIndex);  // Remove from pieces
                delete uploadZone;  // Deallocate memory
            }
        }
        cout << "Upload deletion complete\n";
    }

    cout << "Searching for first program to use\n";
    bool foundFirstProgram = false;
    while (!foundFirstProgram) {  // This could go infinite
        this->currentPlayerIndex++;
        this->currentPlayerIndex = this->currentPlayerIndex % this->players.size();
        // Find a first program
        for (int i=0; i<this->pieces.size(); i++) {
            // Loop through all the pieces, prep all pieces controlled by the player, select one to be first
            DataBattlePiece* piece = this->pieces[i];
            cout << "Type: " << piece->pieceType << '\n';
            cout << "Name: " << piece->name << '\n';
            cout << "Controller: " << piece->controller << '\n';
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
    if (this->currentProgram != nullptr) {
        this->currentProgram->tickStatuses();  // Maybe we want to call this someplace else
        cout << "Statuses ticked\n";
    }
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
            if (piece->pieceType == 'p') {
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

string DataBattle::takeCommand(string command, int playerIndex) {
    cout << "Command: " << command << '\n';
    if (startsWith(command, "upload")) {
        vector<string> splitCommand = splitString(command, ':');
        // 1: Byte coord, 2: Program type, 3: Name
        sf::Vector2i targetCoord = readByteCoord(splitCommand[1]);
        // Do some checking on the coord, make sure there's actually an upload there
        DataBattlePiece* uploadZone = nullptr;
        int uploadIndex = -1;
        for (int i=0; i<this->pieces.size(); i++) {
            DataBattlePiece* piece = this->pieces[i];
            sf::Vector2i coord = piece->sectors[0]->coord;
            if (piece->pieceType == 'u') {
                if ((coord.x == targetCoord.x) && (coord.y == targetCoord.y)) {
                    // Should also check if the player in question controls that upload
                    uploadZone = piece;
                    uploadIndex = i;
                    break;
                }
            }
        }

        if (uploadZone != nullptr) {  // If we found a valid upload zone
            string programType = splitCommand[2];
            Player* player = players[playerIndex];

            for (pair<string, int> p : player->programs) {
                if (programType == p.first) {  // If the player has that program in their inventory
                    if (player->programs[programType] > 0) {  // If they have at least 1
                        player->programs[programType]--;  // Remove 1 from their inventory
                    }
                }
            }

            // Add the program
            Program* newProgram = new Program(PROGRAM_DB[programType]);  // Clone one from PROGRAM_DB
            newProgram->move(targetCoord, true);
            newProgram->owner = playerIndex;
            newProgram->controller = playerIndex;
            if (splitCommand[3] == "NULL") {  // We can add more checks to piece names
                newProgram->name = "piece" + to_string(this->pieceCounter);
            }
            this->addPiece(newProgram);

            // Remove the upload zone
            this->pieces.erase(this->pieces.begin() + uploadIndex); // Remove from pieces
            delete uploadZone;  // Deallocate the memory

            return "ok";
        } else {
            return "upload failed";
        }

    } else if (startsWith(command, "move")) {
        // Do something, Taipu
        // 1: Piece name, 2: direction
        vector<string> splitCommand = splitString(command, ':');
        string pieceName = splitCommand[1];
        char direction = splitCommand[2][0];
        // Find our source piece
        DataBattlePiece* sourcePiece = nullptr;
        for (DataBattlePiece* piece : this->pieces) {
            if (piece->name == pieceName) {
                sourcePiece = piece;
                break;
            }
        }
        if (sourcePiece->currentMove < sourcePiece->speed) {  // If we haven't moved all we can this turn
            // See if that direction is a valid move
            sf::Vector2i coordToCheck = sf::Vector2<int>(sourcePiece->sectors[0]->coord);  // Copy the head coord
            bool valid = false;
            if (direction == 'n') {
                coordToCheck += sf::Vector2<int>(0, -1);
            } else if (direction == 's') {
                coordToCheck += sf::Vector2<int>(0, 1);
            } else if (direction == 'e') {
                coordToCheck += sf::Vector2<int>(1, 0);
            } else if (direction == 'w') {
                coordToCheck += sf::Vector2<int>(-1, 0);
            }
            // See if there's a piece occupying that coord
            DataBattlePiece* occupyingPiece = nullptr;
            for (DataBattlePiece* piece : this->pieces) {
                if ((piece->sectors[0]->coord.x == coordToCheck.x) && (piece->sectors[0]->coord.y == coordToCheck.y)) {
                    occupyingPiece = piece;
                    break;
                }
            }
            if (occupyingPiece != nullptr) {  // If something is occupying that coord
                return "no";  // Fail
                // Implement different stuff for credit pickups, flags, etc.
            }
            // If we're here, there's nothing occupying the coord we want to move on.
            if (this->grid[coordToCheck.x][coordToCheck.y]) {  // If the grid square is not empty
                sourcePiece->move(coordToCheck, false);
                // Add code for gridwalk, negawalk, gridburn
            }
            return "ok";
        }
    } else if (startsWith(command, "action")) {
        // 1: Piece name, 2: Action index, >2: Target coords
        vector<string> splitCommand = splitString(command, ':');
        string pieceName = splitCommand[1];
        int actionIndex = stoi(splitCommand[2]);
        // Do something, Taipu
        // Find our source piece
        DataBattlePiece* sourcePiece = nullptr;
        for (DataBattlePiece* piece : this->pieces) {
            if (piece->name == pieceName) {
                sourcePiece = piece;
                break;
            }
        }
        ProgramAction* action = sourcePiece->actions[actionIndex];
        int maxTargets = action->numOfTargets;
        int i=0;
        // I think it'll be easy to set up a loop to find valid targets.  How do we do something to them?
        while ((i < maxTargets) && (i < splitCommand.size() - 3)) {
            string targetByteCoord = splitCommand[i+3];
            sf::Vector2i targetCoord = readByteCoord(targetByteCoord);
            DataBattlePiece* targetPiece = nullptr;
            for (DataBattlePiece* piece : this->pieces) {
                // See if the target coord is part of that piece's sectors
                for (ProgramSector* sector : piece->sectors) {
                    if ((sector->coord.x == targetCoord.x) && (sector->coord.y == targetCoord.y)) {
                        targetPiece = piece;
                        break;
                    }
                }
                if (targetPiece != nullptr) {  // If we found our target
                    break;
                }
            }
            // Now we need to do something to targetPiece.  Damage, buff/debuff, warp, etc
            // We SHOULD make these command strings as well, so we can pass them across a networked game
            cout << "Hit target\n";

            i++;
        }

        return "ok";

    } else if (startsWith(command, "noaction")) {
        if (startsWith(command, "noaction:")) {  // If the user specified a particular piece
            return "Not implemented";
        } else {  // No piece specified, NA the current piece
            this->currentProgram->noAction();
            this->switchPrograms();
            return "ok";
        }
    } else if (startsWith(command, "DBI")) {
        if (this->currentPlayerIndex == -1) {
            cout << "Checking player readiness\n";
            this->players[playerIndex]->readyup();
            // Check to see if all players are ready; if so, start
            bool allReady = true;
            for (int i=0; i<this->players.size(); i++) {
                Player* player = this->players[i];
                if (!player->ready) {
                    cout << "Player " << i << " not ready\n";
                    allReady = false;
                    break;
                } else {
                    cout << "Player " << i << " ready\n";
                }
            }
            if (allReady) {
                // Start
                cout << "Starting\n";
                this->switchTurns();
            }
            return "ok";
        }
    }

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

void DataBattle::addPiece(DataBattlePiece* newPiece) {
    this->pieces.push_back(newPiece);
    this->pieceCounter++;
}
