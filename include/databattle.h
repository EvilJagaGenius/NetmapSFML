#ifndef DATABATTLE_H
#define DATABATTLE_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <SFML/Graphics.hpp>
using namespace std;

class DataBattle
{
    // Following my style, make everything public until needed otherwise
    public:
        // Variables
        string filename;
        sf::Texture bkg;
        // Insert music file here
        int cashReward;
        vector<int[2]> cashPickups;  // A list of coordinates
        vector<int[2]> uploads;
        int grid[16][16];

        // play()-specific variables here

        // Functions
        DataBattle(string filename);
        virtual ~DataBattle();
        void load();
        sf::Texture render();
        void play(sf::RenderWindow window);

    protected:

    private:
};

#endif // DATABATTLE_H
