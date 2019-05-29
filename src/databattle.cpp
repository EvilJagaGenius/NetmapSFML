#include "databattle.h"

DataBattle::DataBattle(string filename)
{
    //ctor
    this->filename = filename;

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
    ifstream textFile;
    textFile.open("Data\\DataBattles\\" + filename + ".txt");
    string line;
    char loading = '0';  // 0 if we're not loading anything

    int gridY = 0;

    while(getline(textFile, line)) {
        if (startsWith(line, "bkg:")) {  // Background
            string bkgFilename = line.substr(4);
            this->bkg = imgLoad("Data\\DataBattleBkgs\\" + bkgFilename);
        }
        if (startsWith(line, "cash:")) {  // Cash reward
            this->cashReward = stoi(line.substr(5));
        }
        if (startsWith(line, "music:")) {  // Music (nothing right now)
            // Do something, Taipu
        }

        // Grid
        if (startsWith(line, "start_grid")) {
            loading = 'G';
        } if (loading == 'G') {
            if (startsWith(line, "start_grid")) {
                continue;
            } else if (startsWith(line, "end_grid")) {
                loading = '0';
            } else {
                int gridX = 0;
                for (char c : line) {
                    this->grid[gridX][gridY] = c - '0';
                    gridX++;
                }
                gridY++;
            }
        }

        // Commands
        if (startsWith(line, "start_commands")) {
            loading = 'C';
        } if (loading == 'C') {
            if (startsWith(line, "start_commands")) {
                continue;
            } else if (startsWith(line, "end_commands")) {
                loading = '0';
            } else {
                if (startsWith(line, "addUpload")) {  // Upload zone
                    vector<string> splitLine = splitString(line, ':');
                    Coord c(stoi(splitLine[1]), stoi(splitLine[2]));
                    this->uploads.push_back(c);
                }

                if (startsWith(line, "addProgram")) {  // Enemy program
                    vector<string> splitLine = splitString(line, ':');
                    //Program newProgram = new Program(splitLine[1]);
                }

            }
        }
    }
    textFile.close();
}

sf::Texture DataBattle::render() {
    sf::Texture DUMMY_TEXTURE;
    return DUMMY_TEXTURE;
}

void DataBattle::play(sf::RenderWindow window) {

}
