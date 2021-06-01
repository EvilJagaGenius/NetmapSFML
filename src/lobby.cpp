#include "lobby.h"

Lobby::Lobby(string ipString, int port) {
    this->serverSocket = new sf::TcpSocket();
    this->serverSocket->setBlocking(false);
    this->ipString = ipString;
    this->port = port;

    this->textBox = sf::Text("", DEFAULT_FONT, 12);
    this->textBox.setColor(sf::Color::White);
    this->textBox.setPosition(0, 0);

    this->connect(ipString, port);
}

Lobby::Lobby(sf::TcpSocket* socket) {
    this->serverSocket = socket;
    this->ipString = this->serverSocket->getRemoteAddress().toString();
    this->port = this->serverSocket->getRemotePort();  // Either this or getLocalPort(), not sure which

    this->textBox = sf::Text("", DEFAULT_FONT, 12);
    this->textBox.setColor(sf::Color::White);
    this->textBox.setPosition(0, 0);
}

Lobby::~Lobby() {
    // Socket destruction will be handled in main()
}

void Lobby::connect(string ipString, int port) {
    sf::IpAddress ip = sf::IpAddress(ipString);
    sf::Time timeout = sf::seconds(10.00);
    int connectionStatus = serverSocket->connect(ip, port, timeout);
    if (connectionStatus == sf::Socket::Error) {
        cout << "Error while connecting\n";
    } else {
        cout << "Connected\n";
        // Do something, Taipu
        // We need to send data to the server, our name and color
        bool keepReceiving = true;
        while (keepReceiving) {  // Receive data from the server
            sf::Packet receivedPacket = sf::Packet();
            connectionStatus = serverSocket->receive(receivedPacket);
            string packetString;
            receivedPacket >> packetString;
            if (packetString.size() > 0) {
                cout << packetString << '\n';
                /*if (startsWith(packetString, "map:")) {
                    // Do something, Taipu
                    vector<string> splitPacket = splitString(packetString, ':');
                    this->filename = splitPacket[1];
                    cout << splitPacket[1] << '\n';
                    cout << this->filename << '\n';
                    this->load();
                    keepReceiving = false;
                }*/
                // We want to get the DB list and list of players
                if (startsWith(packetString, "dblist:")) {
                    this->dbs.clear();
                    vector<string> dbList = splitString(packetString, ':');
                    for (int i=1; i<dbList.size(); i++) {
                        this->dbs.push_back(dbList[i]);
                    }
                } else if (startsWith(packetString, "players:")) {
                    vector<string> playerList = splitString(packetString, ':');
                    for (int i=1; i<playerList.size(); i++) {
                        cout << playerList[i] << '\n';
                        this->players.push_back(playerList[i]);
                    }
                    keepReceiving = false;  // We can have the server send some sort of done message, but for now this should work
                }
                // More command types here.  Or implement takeCommand()
            }
        }
    }
}

void Lobby::render(sf::RenderWindow* window) {
    window->clear(sf::Color::Blue);  // Blue screen of lobby system kindof working
    int yPos = 0;
    for (int i=0; i<this->dbs.size(); i++) {
        this->textBox.setPosition(0, yPos);
        this->textBox.setString("DB:" + this->dbs[i]);
        window->draw(this->textBox);
        yPos += 14;
    }
    for (int i=0; i<this->players.size(); i++) {
        this->textBox.setPosition(0, yPos);
        this->textBox.setString("PL:" + this->players[i]);
        window->draw(this->textBox);
        yPos += 14;
    }
}

string Lobby::play(sf::RenderWindow* window) {
    // Do something, Taipu
    sf::Vector2i mousePos;
    sf::Event event;
    bool clicked;

    while (window->isOpen()) {
        mousePos = sf::Mouse::getPosition(*window);
        clicked = false;

        while (window->pollEvent(event)) {
            /*if (this->currentInputBox != nullptr) {
                this->currentInputBox->takeInput(event, this);
            }*/
            if (event.type == sf::Event::Closed) {
                window->close();
            } else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::C) {  // Create DB
                    // I have no idea how to format this blasted command
                    // createDB:mapName:credits:Kings:Characters:CTF
                    this->cmdQueue.push("createDB:TestBattle:2000:0:0:0");
                } else if (event.key.code == sf::Keyboard::J) {  // Join DB
                    this->cmdQueue.push("joinDB:0");
                } else if (event.key.code == sf::Keyboard::T) {  // Chat
                    // Do something, Taipu
                    this->cmdQueue.push("chat:Do something, Taipu");
                }
            } else if (event.type == sf::Event::MouseButtonPressed) {
                clicked = true;
            }

            // Something about if we click on a DB, join that DB
        }

        // Receive data from the server
        sf::Packet serverPacket;
        while (this->serverSocket->receive(serverPacket) == sf::Socket::Done) {  // Receive data from the server
            string packetString;
            serverPacket >> packetString;
            if (packetString.size() > 0) {
                cout << packetString << '\n';
                /*if (startsWith(packetString, "map:")) {
                    // Do something, Taipu
                    vector<string> splitPacket = splitString(packetString, ':');
                    this->filename = splitPacket[1];
                    cout << splitPacket[1] << '\n';
                    cout << this->filename << '\n';
                    this->load();
                    keepReceiving = false;
                }*/
                // We want to get the DB list and list of players
                if (startsWith(packetString, "dblist:")) {
                    this->dbs.clear();
                    vector<string> dbList = splitString(packetString, ':');
                    for (int i=1; i<dbList.size(); i++) {
                        this->dbs.push_back(dbList[i]);
                    }
                } else if (startsWith(packetString, "players:")) {
                    vector<string> playerList = splitString(packetString, ':');
                    for (int i=1; i<playerList.size(); i++) {
                        cout << playerList[i] << '\n';
                        this->players.push_back(playerList[i]);
                    }
                } else if (startsWith(packetString, "map:")) {
                    vector<string> splitCommand = splitString(packetString, ':');
                    string mapName = splitCommand[1];
                    return "netdb:" + mapName;
                }
                // More command types here.  Or implement takeCommand()
            }
        }

        // Send data to server
        sf::Packet packetToSend;
        while (!(this->cmdQueue.empty())) {
            packetToSend.clear();
            string command = this->cmdQueue.front();  // Read the command
            cout << "Sending command: " << command << '\n';
            this->cmdQueue.pop();  // Remove from queue
            packetToSend << command;  // Stuff the command into a packet.  Maybe figure out how to stuff multiple commands into a packet
            //commandsInPacket++;
            this->serverSocket->send(packetToSend);
        }

        this->render(window);
        window->display();
    }
    return "quit:";
}
