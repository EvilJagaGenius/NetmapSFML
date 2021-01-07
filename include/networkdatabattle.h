#ifndef NETWORKDATABATTLE_H
#define NETWORKDATABATTLE_H

#include <SFML/Network.hpp>

#include "databattle.h"

// Class for multiplayer DataBattles conducted over a network.
// One local player, connecting to an authoritative server.

class NetworkDataBattle : public DataBattle
{
    public:
        NetworkDataBattle();
        NetworkDataBattle(string ipString, unsigned short port);
        virtual ~NetworkDataBattle();
        void connect(string ipString, unsigned short port);
        void tick();  // Override
        void load();  // Override

        sf::TcpSocket* serverSocket;  // Only maintain one socket, to the server

    protected:

    private:
};

#endif // NETWORKDATABATTLE_H
