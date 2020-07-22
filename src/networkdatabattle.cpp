#include "networkdatabattle.h"

NetworkDataBattle::NetworkDataBattle() {
    // Might have to copy stuff from DataBattle()
    this->serverSocket = new sf::TcpSocket();
    this->serverSocket->setBlocking(false);
}

NetworkDataBattle::NetworkDataBattle(string ipString, unsigned short port) {
    this->serverSocket = new sf::TcpSocket();
    this->serverSocket->setBlocking(false);

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
    this->serverSocket->receive(serverPacket);
    serverPacket >> packetString;
    if (packetString.size() > 0) {
        cout << packetString << '\n';
    }
    // I would love some sort of loop like Java has with scanners.
    // while (serverSocket->hasNext()) {
    //      serverSocket->receive(serverPacket);
    //      Do something, Taipu }
    // How can we mimic that?
    // Once we've received the packet, do whatever it tells you

    // Send our data to the server
    // How do we do this?  Do we need to know which player is the local player and only send their commands?
    /*sf::Packet packetToSend;
    Player* localPlayer = this->players[localPlayerIndex];
    while (!(localPlayer->cmdQueue.empty())) {
        string command = localPlayer->cmdQueue.front();
        localPlayer->cmdQueue.pop();
        this->takeCommand(command, localPlayerIndex);
        packetToSend << command;
        // Is there a maximum size to a packet we might be in danger of exceeding?  Or will the socket handle that for us, splitting things into multiple packets if necessary?
    }
    this->serverSocket->send(packetToSend);*/
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
