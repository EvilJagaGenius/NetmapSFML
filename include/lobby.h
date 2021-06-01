#ifndef LOBBY_H
#define LOBBY_H

#include <SFML/Network.hpp>
#include <queue>
#include "netmap_playable.h"
#include "textinputbox.h"


class Lobby : public Netmap_Playable
{
    public:
        string ipString;
        int port;
        sf::TcpSocket* serverSocket;
        vector<string> dbs;
        vector<string> players;
        queue<string> cmdQueue;
        sf::Text textBox;
        //InputBox* currentInputBox;  // Not sure if we'll use this yet

        Lobby(string ipAddress, int port);
        Lobby(sf::TcpSocket* socket);
        virtual ~Lobby();
        void connect(string ipAddress, int port);
        void render(sf::RenderWindow* window);
        //void tick();
        string play(sf::RenderWindow* window);

    protected:

    private:
};

#endif // LOBBY_H
