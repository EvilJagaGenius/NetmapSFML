#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>
#include <SFML/Graphics.hpp>

using namespace std;

class Player
{
    public:
        // Variables
        unordered_map<string, int> programs;
        int credits;

        // Functions
        Player();
        virtual ~Player();
        void giveStartingPrograms();

    protected:

    private:
};

#endif // PLAYER_H
