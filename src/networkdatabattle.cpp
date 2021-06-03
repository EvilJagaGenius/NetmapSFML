#include "networkdatabattle.h"

NetworkDataBattle::NetworkDataBattle() {
    // Might have to copy stuff from DataBattle()
    this->serverSocket = new sf::TcpSocket();
    this->serverSocket->setBlocking(false);
    this->localPlayerIndex = -1;
    this->currentPlayerIndex = -1;
    this->currentProgram = nullptr;
    this->currentProgramIndex = -1;
    this->nextProgram = nullptr;
    this->nextProgramIndex = -1;
}

NetworkDataBattle::NetworkDataBattle(string filename) {
    // Might have to copy stuff from DataBattle()
    this->filename = filename;
    this->serverSocket = new sf::TcpSocket();
    this->serverSocket->setBlocking(false);
    this->localPlayerIndex = -1;
    this->currentPlayerIndex = -1;
    this->currentProgram = nullptr;
    this->currentProgramIndex = -1;
    this->nextProgram = nullptr;
    this->nextProgramIndex = -1;

    this->load();
}

NetworkDataBattle::NetworkDataBattle(string ipString, unsigned short port) {
    this->serverSocket = new sf::TcpSocket();
    this->serverSocket->setBlocking(false);
    this->localPlayerIndex = -1;
    this->currentPlayerIndex = -1;
    this->currentProgram = nullptr;
    this->currentProgramIndex = -1;
    this->nextProgram = nullptr;
    this->nextProgramIndex = -1;

    this->connect(ipString, port);
}

NetworkDataBattle::~NetworkDataBattle() {
    //delete this->serverSocket;
}

void NetworkDataBattle::connect(string ipString, unsigned short port) {
    sf::IpAddress ip = sf::IpAddress(ipString);
    sf::Time timeout = sf::seconds(10.00);
    int connectionStatus = serverSocket->connect(ip, port, timeout);
    if (connectionStatus == sf::Socket::Error) {
        cout << "Error while connecting\n";
    } else {
        cout << "Connected\n";
        // Do something, Taipu
        // We need to get data from the server - the map name.  Then I think we should be able to take care of most all else in tick()
        bool keepReceiving = true;
        while (keepReceiving) {
            sf::Packet receivedPacket = sf::Packet();
            connectionStatus = serverSocket->receive(receivedPacket);
            string packetString;
            receivedPacket >> packetString;
            if (packetString.size() > 0) {
                cout << packetString << '\n';
                if (startsWith(packetString, "map:")) {
                    // Do something, Taipu
                    vector<string> splitPacket = splitString(packetString, ':');
                    this->filename = splitPacket[1];
                    //cout << splitPacket[1] << '\n';
                    //cout << this->filename << '\n';
                    this->load();
                    keepReceiving = false;
                }
            }
        }
    }
}

void NetworkDataBattle::tick() {  // Override
    //cout << "NetworkDataBattle::tick()\n";
    // Receive data from the server
    sf::Packet serverPacket;
    string packetString;

    //cout << "Receiver loop\n";
    while (this->serverSocket->receive(serverPacket) == sf::Socket::Done) {  // While we're receiving packets from the server
        serverPacket >> packetString;
        if (packetString.size() > 0) {
            cout << "SERVER: " << packetString << '\n';
            // Maybe we should move this split-up into lines into takeCommand()
            /*vector<string> lines = splitString(packetString, '\n');
            for (string line : lines) {
                this->takeCommand(line, this->localPlayerIndex);  // Execute whatever commands the server tells us to
            }*/
            this->takeCommand(packetString, this->localPlayerIndex);
        }
    }

    // Send our data to the server
    if (this->localPlayerIndex != -1) {  // If we're a valid player
        //cout << "Sender loop\n";
        sf::Packet packetToSend;
        //int commandsInPacket = 0;
        Player* localPlayer = this->players[this->localPlayerIndex];
        while (!(localPlayer->cmdQueue.empty())) {  // While there's commands to send
            packetToSend.clear();
            string command = localPlayer->cmdQueue.front();  // Read the command
            cout << "Sending command: " << command << '\n';
            localPlayer->cmdQueue.pop();  // Remove from queue
            //this->takeCommand(command, this->localPlayerIndex);  // Execute on our end... Maybe don't do this, wait for the server to confirm?
            packetToSend << command;  // Stuff the command into a packet
            //commandsInPacket++;
            this->serverSocket->send(packetToSend);
        }
        /*if (commandsInPacket > 0) {
            cout << "Sending packet... ";
            this->serverSocket->send(packetToSend);  // Send that packet containing all of the local player's commands
            cout << "Packet sent\n";
        }*/
    }
}

void NetworkDataBattle::load() {  // Override
    // Like DataBattle::load(), but we only want to load the geometry, background, and music.
    // The server will fill us in on the rest
    cout << "Loading net DB\n";
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
    }
    textFile.close();
    cout << "Done loading net DB\n";
}

string NetworkDataBattle::takeCommand(string command, int playerIndex) {
    cout << "NetCommand: " << command << '\n';
    if (startsWith(command, "upload")) {  // Upload piece
        // 1: Owner, 2: Byte coord, 3: Program type, 4: Name
        vector<string> splitCommand = splitString(command, ':');
        playerIndex = stoi(splitCommand[1]);
        sf::Vector2i targetCoord = readByteCoord(splitCommand[2]);
        // Do some checking on the coord, make sure there's actually an upload there
        DataBattlePiece* uploadZone = nullptr;
        int uploadIndex = -1;
        for (int i=0; i<this->pieces.size(); i++) {
            DataBattlePiece* piece = this->pieces[i];
            sf::Vector2i coord = piece->sectors[0]->coord;
            if (piece->pieceType == 'u' && piece->controller == playerIndex) {
                if ((coord.x == targetCoord.x) && (coord.y == targetCoord.y)) {
                    uploadZone = piece;
                    uploadIndex = i;
                    break;
                }
            }
        }

        if (uploadZone != nullptr) {  // If we found a valid upload zone
            string programType = splitCommand[3];
            Player* player = players[playerIndex];

            for (pair<string, int> p : player->programs) {
                if (programType == p.first) {  // If the player has that program in their inventory
                    if (player->programs[programType] > 0) {  // If they have at least 1
                        player->programs[programType]--;  // Remove 1 from their inventory
                    }
                }
            }

            // Add the program
            Program* newProgram = new Program(programType);  // Create a new one from the definition file
            newProgram->move(targetCoord, true);
            newProgram->owner = playerIndex;
            newProgram->controller = playerIndex;
            newProgram->name = splitCommand[4];
            this->addPiece(newProgram);

            // Remove the upload zone
            this->pieces.erase(this->pieces.begin() + uploadIndex); // Remove from pieces
            delete uploadZone;  // Deallocate the memory

            return "ok";
        } else {
            return "upload failed";
        }

    } else if (startsWith(command, "move")) {  // Move
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
        if (sourcePiece != nullptr) {
            cout << "Moving " << sourcePiece->name << '\n';
            sf::Vector2i coordToCheck = sf::Vector2<int>(sourcePiece->sectors[0]->coord);  // Copy the head coord
            if (direction == 'n') {
                coordToCheck += sf::Vector2<int>(0, -1);
            } else if (direction == 's') {
                coordToCheck += sf::Vector2<int>(0, 1);
            } else if (direction == 'e') {
                coordToCheck += sf::Vector2<int>(1, 0);
            } else if (direction == 'w') {
                coordToCheck += sf::Vector2<int>(-1, 0);
            }
            if (this->grid[coordToCheck.x][coordToCheck.y]) {  // If the grid square is not empty
                sourcePiece->move(coordToCheck, false);
                // Add code for gridwalk, negawalk, gridburn
                if (sourcePiece->currentMove >= sourcePiece->speed) {
                    sourcePiece->switchToAiming();
                }
            }
            return "ok";
        } else {
            cout << "sourcePiece not found\n";
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
        //cout << sourcePiece->actions.size() << '\n';
        ProgramAction* action = sourcePiece->actions[actionIndex];
        int maxTargets = action->numOfTargets;
        int i=0;
        // Get target coords
        vector<sf::Vector2i> targetCoords;
        while ((i < maxTargets) && (i < splitCommand.size() - 3)) {
            string targetByteCoord = splitCommand[i+3];
            sf::Vector2i targetCoord = readByteCoord(targetByteCoord);
            targetCoords.push_back(targetCoord);
            i++;
        }
        // Perform the action on those targets
        this->performAction(action, targetCoords);

        // Once we've used that action, that piece is done
        sourcePiece->noAction();
        if (sourcePiece == this->currentProgram) {  // Switch programs if necessary
            this->switchPrograms();
        }

        return "ok";

    } else if (startsWith(command, "noaction")) {
        if (startsWith(command, "noaction:")) {  // If the user specified a particular piece, hence the semicolon
            return "Not implemented";
        } else {  // No piece specified, NA the current piece
            this->currentProgram->noAction();
            this->switchPrograms();
            return "ok";
        }
    } else if (startsWith(command, "DBI")) {  // Databattle Initialize
        cout << "Starting\n";
        // We need to destroy the upload map too
        this->switchTurns();  // We might need to replace this with different commands
        return "ok";

    } else if (startsWith(command, "players:")) {
        // Create that many players and add them to the game
        int numberOfPlayers = stoi(command.substr(8));
        this->players.clear();  // Empty out whatever players we had.  In a network game, this empties out PLAYER (we still have a reference to it though in main.cpp)
        for (int i=0; i<numberOfPlayers; i++) {
            Player* newPlayer = new Player();  // Create new player object
            this->players.push_back(newPlayer);  // Add it to the game
        }
        // We're going to need some way to clean all these players up
        return "ok";

    } else if (startsWith(command, "playerIndex:")) {  // Set localPlayerIndex
        this->localPlayerIndex = stoi(command.substr(12));
        return "ok";

    } else if (startsWith(command, "addUpload:")) {  // Upload zone
        // 1: Byte coord, 2: owner
        cout << "Adding upload zone... ";
        vector<string> splitCommand = splitString(command, ':');
        sf::Vector2i coord = readByteCoord(splitCommand[1]);
        UploadZone* newUpload = new UploadZone(coord.x, coord.y, stoi(splitCommand[2]));
        this->addPiece(newUpload);
        cout << "Done\n";
    } else if (startsWith(command, "addProgram:")) {
        // 1: Program type, 2: byte coord, 3: owner, 4: name
        cout << "Adding program\n";
        vector<string> splitCommand = splitString(command, ':');
        Program* newProgram = new Program(splitCommand[1]);
        sf::Vector2i coord = readByteCoord(splitCommand[2]);
        newProgram->move(coord, true);
        newProgram->owner = stoi(splitCommand[3]);
        newProgram->name = splitCommand[4];
        this->addPiece(newProgram);
    } else if (startsWith(command, "startTurn:")) {  // Start turn
        // 1: Player index
        // What do we do here?  We need a targeted version of switchTurns()
        playerIndex = stoi(command.substr(10));
        this->currentPlayerIndex = playerIndex;
        bool foundFirstProgram = false;
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
        this->switchPrograms();
    }

    return "Not implemented";
}

