#ifndef NETWORKDATABATTLE_H
#define NETWORKDATABATTLE_H

#include <SFML/Network.hpp>

#include "databattle.h"

// Class for multiplayer DataBattles conducted over a network.
// One local player, connecting to an authoritative server.

class NetworkDataBattle : public DataBattle
{
    public:
        sf::TcpSocket* serverSocket;  // Only maintain one socket, to the server

        NetworkDataBattle();
        NetworkDataBattle(string filename);
        NetworkDataBattle(string ipString, unsigned short port);
        virtual ~NetworkDataBattle();
        void connect(string ipString, unsigned short port);
        void tick();  // Override
        void load();  // Override
        string takeCommand(string command, int playerIndex);

    protected:

    private:
};

#endif // NETWORKDATABATTLE_H
