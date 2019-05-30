#include "program.h"

Program::Program(string programType) {
    this->programType = programType;
    this->load();
}

Program::~Program()
{
    //dtor
}

void Program::load() {
    ifstream textFile;
    textFile.open("Data\\Programs\\" + programType + ".txt");
    string line;
    vector<string> splitLine;
    while (getline(textFile, line)) {
        splitLine = splitString(line, ':');
        if (startsWith(line, "name")) {
            this->screenName = splitLine[1];
        } else if (startsWith(line, "maxSize")) {
            this->maxSize = stoi(splitLine[1]);
        } else if (startsWith(line, "speed")) {
            this->speed = stoi(splitLine[1]);
        } else if (startsWith(line, "maxSpeed")) {
            this->maxSpeed = stoi(splitLine[1]);
        } else if (startsWith(line, "action")) {
            // Do something, Taipu
        } else if (startsWith(line, "sprite")) {
            this->spriteCoord = *(new Coord(stoi(splitLine[1]), stoi(splitLine[2])));
        } else if (startsWith(line, "maxSize")) {
            this->color = *(new sf::Color(stoi(splitLine[1]), stoi(splitLine[2]), stoi(splitLine[3])));
        } else if (startsWith(line, "description")) {
            this->description = splitLine[1];
        }
    }
}

void Program::move(Coord c, bool firstTime) {

}

void Program::addSector(Coord c) {

}

void Program::useAction(int actionIndex, Coord targetCoord) {

}

void Program::switchToAiming(int actionIndex) {

}

void Program::takeDamage(DataBattle* level, int damage) {

}

void Program::grow(int amtToGrow) {

}

void prepForTurn() {

}
