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
#include "databattleplayer.h"
//#include "databattleeditor.h"  // Needs reworked before adding it back in
#include "titlescreen.h"
//#include "hud.h"  // This might need reworked too
#include "scene.h"
#include "npc.h"

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
        } else if (startsWith(nextPlayable, "scene:")) {
            cout << "Launching scene\n";
            CURRENT_PLAYABLE = new Scene(splitString(nextPlayable, ':')[1]);
        } else if (startsWith(nextPlayable, "npc:")) {
            cout << "Launching NPC\n";
            vector<string> splitPlayable = splitString(nextPlayable, ':');
            CURRENT_PLAYABLE = new NPC(splitPlayable[1]);
            CURRENT_PLAYABLE->destination = lastPlayable;
        } else if (startsWith(nextPlayable, "netgame:")) {
            cout << "Network game\n";
            vector<string> splitAddress = splitString(nextPlayable, ':');
            string ipAddress = splitAddress[1];
            unsigned short portNum = stoi(splitAddress[2]);
            // Do something, Taipu
            db = new NetworkDataBattle(ipAddress, portNum);
            CURRENT_PLAYABLE = new DataBattlePlayer(db);
        }
        lastPlayable = nextPlayable;
    }
    cout << "Loop exited\n";

    // We need to clean things up before we exit the program
    delete PLAYER;
    delete PLAYER2;

    cout << "Cleanup complete\n";

    return 0;
}
