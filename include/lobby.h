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
        vector<string> chatbox;  // I would like to use a queue, but queues do not allow for element access and I don't feel like coding around it
        int maxChatboxSize = 20;
        queue<string> cmdQueue;
        sf::Text textBox;
        sf::Vector2i mousePos;
        sf::Rect<int> buttonRect;
        sf::RectangleShape buttonGraphic;
        int highlightedDB;

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
