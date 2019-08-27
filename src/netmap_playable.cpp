#include "netmap_playable.h"

Netmap_Playable::Netmap_Playable()
{
    for (int i=0; i<16; i++) {
        for (int j=0; j<16; j++) {
            this->grid[i][j] = 0;
        }
    }
}

Netmap_Playable::~Netmap_Playable()
{
    //dtor
}

void Netmap_Playable::render(sf::RenderWindow* window) {

}

string Netmap_Playable::play(sf::RenderWindow* window) {
    cout << "Called Netmap_Playable::play()\n";
}
void Netmap_Playable::setHUD(InputBox* hud) {}
void Netmap_Playable::setPlayer(Player* player) {}
string Netmap_Playable::takeCommand(string command) {
    return "";
}
string Netmap_Playable::lookAt(sf::Vector2i coord) {
    return "";
}
void Netmap_Playable::flipSector(sf::Vector2i coord) {}
