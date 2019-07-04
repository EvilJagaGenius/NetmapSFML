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
            this->actions.push_back(&ACTION_DB[splitLine[1]]);
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
        bool doublingBack = false;
        int doubleBackIndex = -1;
        for (int i=0; i<this->sectors.size(); i++) {
            if (this->sectors[i].coord == coord) {
                doublingBack = true;
                doubleBackIndex = i;
            }
        }

        if (doublingBack) {
            // Swap the positions of the current head and the sector we're going to
            ProgramSector doubleBackSector = this->sectors[doubleBackIndex];
            this->sectors[doubleBackIndex] = this->sectors[0];
            this->sectors[0] = doubleBackSector;
            this->currentMove++;
        } else {
            this->sectors.insert(this->sectors.begin(), ProgramSector(coord, this->sectors[0]));  // Add new sector as the head
            while (this->sectors.size() > this->maxSize) {  // Trim program down if necessary
                for (int i=0; i<this->sectors.size(); i++) {
                    ProgramSector s = this->sectors[i];
                    if ((s.numLinks == 1) && (i != 0)) {
                        ProgramSector connectedSector = s.links[0];
                        // Unlink the sectors
                        for (int j=0; j<connectedSector.links.size(); j++) {
                            if (connectedSector.links[j].coord == s.coord) {
                                connectedSector.links.erase(connectedSector.links.begin() + i);
                                break;
                            }
                        }
                        connectedSector.numLinks--;
                        break;
                    }
                }
            }
            this->currentMove++;
        }
        this->size = this->sectors.size();
    }
}

void Program::addSector(sf::Vector2i coord, int pos=0) {
    vector<ProgramSector>::iterator itr = sectors.begin();
    advance(itr, pos);
    ProgramSector newSector = ProgramSector(coord, sectors[pos]);
    sectors.push_back(newSector);
    this->size++;
}

void Program::useAction(Netmap_Playable* level, int actionIndex, sf::Vector2i targetCoord) {
    this->actions[actionIndex]->use(level, this, targetCoord);
    // Action is used whether it failed or not, end the turn
    this->state = 'd';
}

void Program::switchToAiming(int actionIndex) {
    this->state = 'a';
    this->currentActionIndex = actionIndex;
    this->currentAction = this->actions[this->currentActionIndex];
    // Should add an if statement to automatically end the turn if we don't have any actions (like the Memory Hog)
}

void Program::takeDamage(Netmap_Playable* level, int damage) {

}

void Program::grow(Netmap_Playable* level, int amtToGrow) {

}

void Program::prepForTurn() {
    this->state = 'm';
    this->currentActionIndex = -1;
    this->currentMove = 0;
}
