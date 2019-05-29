#include "databattle.h"

DataBattle::DataBattle(string filename)
{
    //ctor

    for (int i=0; i<16; i++) {
        for (int j=0; j<16; j++) {
            this->grid[i][j] = 0;
        }
    }

    this->load();
}

DataBattle::~DataBattle()
{
    //dtor
    // Remember to fill this in
}

void DataBattle::load() {
    // Load text file, print each line for now
    // Do something, Taipu
    ifstream textFile;
    textFile.open("TestFile.txt");
    string line;
    while(getline(textFile, line)) {
        cout << line << '\n';
    }
    textFile.close();
}

sf::Texture DataBattle::render() {

}

void DataBattle::play(sf::RenderWindow window) {

}
