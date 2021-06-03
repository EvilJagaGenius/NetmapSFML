// Main file for Netmap 1.0

#include <iostream>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include "toolbox.h"
#include "netmap_playable.h"
#include "networkdatabattle.h"
#include "lobby.h"
#include "databattleplayer.h"
//#include "databattleeditor.h"  // Needs reworked before adding it back in
#include "titlescreen.h"
//#include "hud.h"  // This might need reworked too
#include "scene.h"
#include "npc.h"
#include "shopinputbox.h"

int main()
{
	cout << "Start.\n";

    sf::RenderWindow window(sf::VideoMode(1024, 576), "NETMAP 1.0");
    window.setFramerateLimit(60);

    Player* PLAYER = new Player();
    Player* PLAYER2 = new Player();
    PLAYER->color = sf::Color::Red;
    PLAYER2->color = sf::Color::Blue;

    Netmap_Playable* CURRENT_PLAYABLE = new TitleScreen();
    DataBattle* db = nullptr;
    sf::TcpSocket* netSocket = nullptr;  // For netplay shenanigans
    string nextPlayable;
    string lastPlayable = "quit:"; //"scene:testScene";
    // There should probably be a better way to connect players and DB Players.
    //CURRENT_PLAYABLE->play(&window);
    while (true) {
        CURRENT_PLAYABLE->setPlayer(PLAYER);
        nextPlayable = CURRENT_PLAYABLE->play(&window);
        cout << nextPlayable << '\n';
        delete CURRENT_PLAYABLE;
        if (db != nullptr) {
            delete db;
            db = nullptr;
        }
        cout << "Deletion of CURRENT_PLAYABLE successful\n";
        if (startsWith(nextPlayable, "quit:") || (nextPlayable.size() == 0)) {
            cout << "Quitting game\n";
            break;
        } else if (startsWith(nextPlayable, "db:")) {
            cout << "Launching DB\n";
            db = new DataBattle(splitString(nextPlayable, ':')[1]);
            CURRENT_PLAYABLE = new DataBattlePlayer(db);
        } else if (startsWith(nextPlayable, "title:")) {
            cout << "Returning to title\n";
            CURRENT_PLAYABLE = new TitleScreen();
            if (netSocket != nullptr) {  // If we get to the title screen, we have left the netgame lobby
                delete netSocket;
                netSocket = nullptr;
            }
        } else if (startsWith(nextPlayable, "scene:")) {
            cout << "Launching scene\n";
            CURRENT_PLAYABLE = new Scene(splitString(nextPlayable, ':')[1]);
        } else if (startsWith(nextPlayable, "npc:")) {
            cout << "Launching NPC\n";
            vector<string> splitPlayable = splitString(nextPlayable, ':');
            CURRENT_PLAYABLE = new NPC(splitPlayable[1]);
            CURRENT_PLAYABLE->destination = lastPlayable;
        } else if (startsWith(nextPlayable, "netlobby:")) {
            cout << "Network lobby\n";
            vector<string> splitAddress = splitString(nextPlayable, ':');
            string ipAddress = splitAddress[1];
            unsigned short portNum = stoi(splitAddress[2]);
            Lobby* newLobby;
            if (netSocket == nullptr) {
                Lobby* newLobby = new Lobby(ipAddress, portNum);
                netSocket = newLobby->serverSocket;  // If we jump into a DB from here, we can pass the socket along
            } else {
                Lobby* newLobby = new Lobby(netSocket);
            }
            CURRENT_PLAYABLE = newLobby;
        } else if (startsWith(nextPlayable, "netdb:")) {
            vector<string> splitPlayable = splitString(nextPlayable, ':');
            NetworkDataBattle* newNetDB = new NetworkDataBattle(splitPlayable[1]);
            newNetDB->serverSocket = netSocket;
            db = newNetDB;
            DataBattlePlayer* dbPlayer = new DataBattlePlayer(db);
            dbPlayer->destination = "netlobby:" + netSocket->getRemoteAddress().toString() + to_string(netSocket->getRemotePort());
            dbPlayer->inputBox = new ShopInputBox();
            CURRENT_PLAYABLE = dbPlayer;
        }
        lastPlayable = nextPlayable;
    }
    cout << "Loop exited\n";

    // We need to clean things up before we exit the program
    delete PLAYER;
    delete PLAYER2;
    if (netSocket != nullptr) {
        delete netSocket;
        netSocket = nullptr;
    }

    cout << "Cleanup complete\n";

    return 0;
}
