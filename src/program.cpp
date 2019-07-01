#include "program.h"

Program::Program(string programType) {
    this->programType = programType;
    this->color = sf::Color::White;
    this->load();
}

Program::Program(Program* original) {  // Copy constructor
    // Do something, Taipu
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
            this->name = splitLine[1];
            this->screenName = splitLine[1];
        } else if (startsWith(line, "screenName")) {
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
            this->spriteCoord = *(new sf::Vector2i(stoi(splitLine[1]), stoi(splitLine[2])));
        } else if (startsWith(line, "maxSize")) {
            this->color = *(new sf::Color(stoi(splitLine[1]), stoi(splitLine[2]), stoi(splitLine[3])));
        } else if (startsWith(line, "description")) {
            this->description = splitLine[1];
        } else if (startsWith(line, "color")) {
            this->color = *(new sf::Color(stoi(splitLine[1]), stoi(splitLine[2]), stoi(splitLine[3])));
        }
    }
}

void Program::move(sf::Vector2i coord, bool firstTime=false) {
    if (firstTime) {
        sectors = *(new vector<ProgramSector>);
        sectors.push_back(*(new ProgramSector(coord)));
        this->size = 1;
    } else {
        ProgramSector newSector = *(new ProgramSector(coord, sectors[0]));
        vector<ProgramSector>::iterator itr = sectors.begin();
        sectors.insert(itr, newSector);
        this->size++;
    }
}

void Program::addSector(sf::Vector2i coord, int pos=0) {
    vector<ProgramSector>::iterator itr = sectors.begin();
    advance(itr, pos);
    ProgramSector newSector = *(new ProgramSector(coord, sectors[pos]));
    sectors.push_back(newSector);
    this->size++;
}

void Program::useAction(DataBattle* level, int actionIndex, sf::Vector2i targetCoord) {

}

void Program::switchToAiming(int actionIndex) {

}

void Program::takeDamage(DataBattle* level, int damage) {

}

void Program::grow(DataBattle* level, int amtToGrow) {

}

void Program::prepForTurn() {

}
