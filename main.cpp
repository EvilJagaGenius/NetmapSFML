#include <iostream>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "toolbox.h"
#include "programaction.h"
#include "databattle.h"
#include "databattleeditor.h"
#include "titlescreen.h"
#include "hud.h"
#include "scene.h"
#include "npc.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1024, 576), "NETMAP 1.0");
    window.setFramerateLimit(60);

    HUD* testHUD = new HUD();
    Player* PLAYER = new Player;
    testHUD->setPlayer(PLAYER);

    Netmap_Playable* CURRENT_PLAYABLE = new TitleScreen();
    //Netmap_Playable* CURRENT_PLAYABLE = new Scene("RNA_1");  // Testing out scenes
    //Netmap_Playable* CURRENT_PLAYABLE = new NPC("RNA_Gemma_1");
    string nextPlayable;
    string lastPlayable = "quit:"; //"scene:testScene";
    //Netmap_Playable* CURRENT_PLAYABLE = new DataBattle("TestBattle");
    while (true) {
        CURRENT_PLAYABLE->setHUD(testHUD);
        CURRENT_PLAYABLE->setPlayer(PLAYER);
        nextPlayable = CURRENT_PLAYABLE->play(&window);
        delete CURRENT_PLAYABLE;
        cout << "Deletion of CURRENT_PLAYABLE successful\n";
        if (startsWith(nextPlayable, "quit:") || (nextPlayable.size() == 0)) {
            cout << "Quitting game\n";
            break;
        } else if (startsWith(nextPlayable, "editor:")) {
            cout << "Launching editor\n";
            CURRENT_PLAYABLE = new DataBattleEditor(splitString(nextPlayable, ':')[1]);
        } else if (startsWith(nextPlayable, "db:")) {
            cout << "Launching DB\n";
            CURRENT_PLAYABLE = new DataBattle(splitString(nextPlayable, ':')[1]);
        } else if (startsWith(nextPlayable, "dbFromEditor:")) {
            cout << "Launching DB from editor\n";
            CURRENT_PLAYABLE = new DataBattle(splitString(nextPlayable, ':')[1]);
            CURRENT_PLAYABLE->destination = "editor:" + splitString(nextPlayable, ':')[1];
            cout << "Creation successful\n";
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
        }
        lastPlayable = nextPlayable;
    }
    cout << "Loop exited\n";

    // We need to clean things up before we exit the program
    delete PLAYER;
    delete testHUD;
    for (pair<string, Program*> p : PROGRAM_DB) {
        delete p.second;
    }
    for (pair<string, ProgramAction*> p : ACTION_DB) {
        delete p.second;
    }
    cout << "Cleanup complete\n";

    return 0;
}
