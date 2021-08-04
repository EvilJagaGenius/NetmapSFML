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
        int serverWinner;

        NetworkDataBattle();
        NetworkDataBattle(string filename);
        NetworkDataBattle(string ipString, unsigned short port);
        virtual ~NetworkDataBattle();
        void connect(string ipString, unsigned short port);
        void tick();  // Override
        void load();  // Override
        string takeCommand(string command, int playerIndex);  // Override
        int checkForVictory();  // Override

    protected:

    private:
};

#endif // NETWORKDATABATTLE_H
