#include "lobby.h"

Lobby::Lobby(string ipString, int port) {
    this->serverSocket = new sf::TcpSocket();
    this->serverSocket->setBlocking(false);
    this->ipString = ipString;
    this->port = port;

    this->textBox = sf::Text("", DEFAULT_FONT, 12);
    this->textBox.setColor(sf::Color::White);
    this->textBox.setPosition(0, 0);
    this->buttonRect = sf::Rect<int>(0, 0, 200, 14);
    this->buttonGraphic = sf::RectangleShape(sf::Vector2<float>(200, 14));
    this->buttonGraphic.setFillColor(sf::Color::Transparent);
    this->buttonGraphic.setOutlineColor(sf::Color::White);
    this->buttonGraphic.setOutlineThickness(1);

    this->inputBox = nullptr;
    this->inputBoxType = '0';

    this->connect(ipString, port);
}

Lobby::Lobby(sf::TcpSocket* socket) {
    this->serverSocket = socket;
    this->ipString = this->serverSocket->getRemoteAddress().toString();
    this->port = this->serverSocket->getRemotePort();  // Either this or getLocalPort(), not sure which

    this->textBox = sf::Text("", DEFAULT_FONT, 12);
    this->textBox.setColor(sf::Color::White);
    this->textBox.setPosition(0, 0);
    this->buttonRect = sf::Rect<int>(0, 0, 200, 14);
    this->buttonGraphic = sf::RectangleShape(sf::Vector2<float>(200, 14));
    this->buttonGraphic.setFillColor(sf::Color::Transparent);
    this->buttonGraphic.setOutlineColor(sf::Color::White);
    this->buttonGraphic.setOutlineThickness(1);

    this->inputBox = nullptr;
    this->inputBoxType = '0';
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
        /*bool keepReceiving = true;
        while (keepReceiving) {  // Receive data from the server
            sf::Packet receivedPacket = sf::Packet();
            connectionStatus = serverSocket->receive(receivedPacket);
            string packetString;
            receivedPacket >> packetString;
            if (packetString.size() > 0) {
                cout << packetString << '\n';
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
        }*/
    }
}

void Lobby::render(sf::RenderWindow* window) {
    window->clear(sf::Color::Black);
    this->highlightedDB = -1;

    this->textBox.setPosition(0, 0);
    this->textBox.setString("DataBattles:");
    window->draw(this->textBox);

    this->buttonRect.left = 0;
    int yPos = 14;
    for (int i=0; i<this->dbs.size(); i++) {
        this->buttonRect.top = yPos;
        if (this->buttonRect.contains(mousePos)) {
            this->highlightedDB = i;
            this->buttonGraphic.setPosition(0, yPos);
            window->draw(this->buttonGraphic);
        }
        this->textBox.setPosition(0, yPos);
        this->textBox.setString("DB:" + this->dbs[i]);
        window->draw(this->textBox);
        yPos += 14;
    }

    this->textBox.setPosition(0, 150);
    this->textBox.setString("Chat:");
    window->draw(this->textBox);

    yPos = 400;
    for (int i=0; i<this->chatbox.size(); i++) {
        this->textBox.setPosition(0, yPos);
        this->textBox.setString(this->chatbox[i]);
        window->draw(this->textBox);
        yPos -= 14;
    }

    this->textBox.setPosition(WY, 0);
    this->textBox.setString("Players:");
    window->draw(this->textBox);
    yPos = 14;
    for (int i=0; i<this->players.size(); i++) {
        this->textBox.setPosition(WY, yPos);
        this->textBox.setString("PL:" + this->players[i]);
        window->draw(this->textBox);
        yPos += 14;
    }

    // Render an input box if we have one
    if (this->inputBox != nullptr) {
        this->inputBox->render(window, this);
    }
}

string Lobby::play(sf::RenderWindow* window) {
    sf::Event event;
    bool clicked;
    string dbMapName;
    int dbCreditLimit;

    while (window->isOpen()) {
        this->mousePos = sf::Mouse::getPosition(*window);
        clicked = false;

        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window->close();
            }
            if (this->inputBox != nullptr) {
                this->inputBox->takeInput(event, this);
            } else {
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::C) {  // Create DB
                        // createDB:mapName:credits:Kings:Characters:CTF
                        this->inputBox = new TextInputBox("Map name (use TestMPBattle):");
                        this->inputBoxType = 'm';
                    } else if (event.key.code == sf::Keyboard::J) {  // Join DB
                        this->cmdQueue.push("joinDB:0");
                    } else if (event.key.code == sf::Keyboard::T) {  // Chat
                        // Do something, Taipu
                        this->inputBox = new TextInputBox("Send chat message:");
                        this->inputBoxType = 't';
                    }
                } else if ((event.type == sf::Event::MouseButtonPressed) && (event.mouseButton.button == sf::Mouse::Left)) {
                    clicked = true;
                }
            }
        }
        // Input box stuff
        if (this->inputBox != nullptr) {
            if (this->inputBox->done) {
                if (this->inputBoxType == 'm') {  // Map name
                    dbMapName = this->inputBox->getFocus();
                    delete this->inputBox;
                    this->inputBox = new TextInputBox("Credit limit:");
                    this->inputBoxType = 'c';
                } else if (this->inputBoxType == 'c') {  // Credit limit
                    dbCreditLimit = stoi(this->inputBox->getFocus());
                    delete this->inputBox;
                    this->inputBox = nullptr;
                    this->inputBoxType = '0';
                    // We have all the info we need (for now), send a message to the server
                    this->cmdQueue.push("createDB:" + dbMapName + ":" + to_string(dbCreditLimit) + ":1:0:0");
                } else if (this->inputBoxType == 't') {
                    string message = this->inputBox->getFocus();
                    delete this->inputBox;
                    this->inputBox = nullptr;
                    this->inputBoxType = '0';
                    this->cmdQueue.push("chat:" + message);
                }
            }
        }


        if (clicked && (highlightedDB != -1)) {  // If the user clicked on a DB, join that DB
            this->cmdQueue.push("joinDB:" + highlightedDB);
        }

        // Receive data from the server
        sf::Packet serverPacket;
        while (this->serverSocket->receive(serverPacket) == sf::Socket::Done) {  // Receive data from the server
            string packetString;
            serverPacket >> packetString;
            if (packetString.size() > 0) {
                cout << packetString << '\n';
                if (startsWith(packetString, "dblist:")) {  // Get DB list
                    this->dbs.clear();
                    vector<string> dbList = splitString(packetString, ':');
                    for (int i=1; i<dbList.size(); i++) {
                        this->dbs.push_back(dbList[i]);
                    }
                } else if (startsWith(packetString, "players:")) {  // Get player list
                    vector<string> playerList = splitString(packetString, ':');
                    for (int i=1; i<playerList.size(); i++) {
                        cout << playerList[i] << '\n';
                        this->players.push_back(playerList[i]);
                    }
                } else if (startsWith(packetString, "netdb:")) {  // Jump into a DB
                    //vector<string> splitCommand = splitString(packetString, ':');
                    //string mapName = splitCommand[1];
                    return packetString;
                } else if (startsWith(packetString, "chat:")) {  // Chat messages
                    // Do something, Taipu
                    this->chatbox.insert(this->chatbox.begin(), packetString.substr(5));
                    if (chatbox.size() > maxChatboxSize) {
                        chatbox.pop_back();
                    }
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
