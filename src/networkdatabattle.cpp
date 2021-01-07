#include "networkdatabattle.h"

NetworkDataBattle::NetworkDataBattle() {
    // Might have to copy stuff from DataBattle()
    this->serverSocket = new sf::TcpSocket();
    this->serverSocket->setBlocking(false);
    this->localPlayerIndex = -1;
    this->currentPlayerIndex = -1;
}

NetworkDataBattle::NetworkDataBattle(string ipString, unsigned short port) {
    this->serverSocket = new sf::TcpSocket();
    this->serverSocket->setBlocking(false);
    this->localPlayerIndex = -1;
    this->currentPlayerIndex = -1;

    this->connect(ipString, port);
}

NetworkDataBattle::~NetworkDataBattle() {
    delete this->serverSocket;
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
                    cout << splitPacket[1] << '\n';
                    cout << this->filename << '\n';
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

    // I would love some sort of loop like Java has with scanners.
    // while (serverSocket->hasNext()) {
    //      serverSocket->receive(serverPacket);
    //      Do something, Taipu }
    // How can we mimic that?
    // Once we've received the packet, do whatever it tells you
    //cout << "Receiver loop\n";
    while (this->serverSocket->receive(serverPacket) == sf::Socket::Done) {  // While we're receiving packets from the server
        serverPacket >> packetString;
        if (packetString.size() > 0) {
            cout << packetString << '\n';
            // Maybe we should move this split-up into lines into takeCommand()
            vector<string> lines = splitString(packetString, '\n');
            for (string line : lines) {
                this->takeCommand(line, this->localPlayerIndex);  // Execute whatever commands the server tells us to
            }
        }
    }

    // Send our data to the server
    if (this->localPlayerIndex != -1) {  // If we're a valid player
        //cout << "Sender loop\n";
        sf::Packet packetToSend;
        Player* localPlayer = this->players[localPlayerIndex];
        int commandsInPacket = 0;
        while (!(localPlayer->cmdQueue.empty())) {  // While there's commands to send
            string command = localPlayer->cmdQueue.front();  // Read the command
            localPlayer->cmdQueue.pop();  // Remove from queue
            this->takeCommand(command, localPlayerIndex);  // Execute on our end... Maybe don't do this, wait for the server to confirm?
            packetToSend << (command + "\n");  // Stuff the command into a packet
            commandsInPacket++;
            // Is there a maximum size to a packet we might be in danger of exceeding?  Or will the socket handle that for us, splitting things into multiple packets if necessary?
        }
        if (commandsInPacket > 0) {
            //cout << "Sending packet... ";
            this->serverSocket->send(packetToSend);  // Send that packet containing all of the local player's commands
            //cout << "Packet sent\n";
        }
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
