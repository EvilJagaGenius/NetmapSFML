#include "player.h"

Player::Player()
{
    //ctor

    this->giveStartingPrograms();
}

Player::~Player()
{
    //dtor
}

void Player::giveStartingPrograms() {
    // Debug mode
    this->programs.insert({{"Hack", 10},
                          {"Slingshot", 10},
                          {"DataDr", 10},
                          {"BitMan", 10},
                          {"Bug", 10},
                          {"Watchman", 10}});
}

void Player::readyup() {
    this->ready = true;
}

void Player::addToUploadMap(string byteCoord, string pieceName) {
    cout << "Adding piece to upload map\n";
    // Do something, Taipu
    // Put that piece in uploadMap, so we upload it on hitting DBI
    bool foundUpload = false;
    // See if we already have a piece ready to upload at that coord
    for (pair<string, string> p : this->uploadMap) {
        if (p.first == byteCoord) {  // If so
            cout << "Replacing piece\n";
            // Replace it
            foundUpload = true;
            this->uploadMap[p.first] = pieceName;
            break;
        }
    }
    if (!foundUpload) {  // Otherwise
        cout << "Creating spot in upload map\n";
        // Create an entry for that spot in uploadMap
        this->uploadMap.emplace(byteCoord, pieceName);
    }
}
