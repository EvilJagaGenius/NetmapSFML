#include "program.h"

Program::Program(string programType) {
    this->programType = programType;
    this->color = sf::Color::White;
    this->load();
}

Program::Program(Program* original) {  // Copy constructor
    this->programType = original->programType;
    this->screenName = original->screenName;
    this->spriteCoord = original->spriteCoord;
    this->color = original->color;

    for (ProgramSector* s : original->sectors) {
        this->sectors.push_back(new ProgramSector(s->coord));
    }

    for (int i=0; i<this->sectors.size(); i++) {
        ProgramSector* s = this->sectors[i];
        vector<sf::Vector2i> linkCoords;
        vector<int> linkIndices;
        for (ProgramSector* l : s->links) {
            linkCoords.push_back(l->coord);
            for (int j=0; j<this->sectors.size(); j++) {
                if (this->sectors[j]->coord == l->coord) {
                    linkIndices.push_back(j);
                }
            }
        }
        for (int index : linkIndices) {
            ProgramSector* sectorToLink = this->sectors[index];
            ProgramSector::linkSectors(s, sectorToLink);
        }
    }

    this->actions = original->actions;
    this->description = original->description;
    this->size = original->size;
    this->maxSize = original->maxSize;
    this->speed = original->speed;
    this->maxSpeed = original->speed;
    this->currentMove = original->currentMove;
    this->state = original->state;


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
    //cout << "Moving\n";
    if (firstTime) {
        sectors = *(new vector<ProgramSector*>);
        sectors.push_back(new ProgramSector(coord));
        this->size = 1;
    } else {
        bool doublingBack = false;
        int doubleBackIndex = -1;
        for (int i=0; i<this->sectors.size(); i++) {
            if (this->sectors[i]->coord == coord) {
                doublingBack = true;
                doubleBackIndex = i;
            }
        }

        if (doublingBack) {
            //cout << "Doubling back\n";
            // Swap the positions of the current head and the sector we're going to
            ProgramSector* doubleBackSector = this->sectors[doubleBackIndex];
            this->sectors[doubleBackIndex] = this->sectors[0];
            this->sectors[0] = doubleBackSector;
            this->currentMove++;
        } else {
            cout << "Not doubling back\n";
            this->sectors.insert(this->sectors.begin(), new ProgramSector(coord, this->sectors[0]));  // Add new sector as the head
            cout << "Added new head\n";
            while (this->sectors.size() > this->maxSize) {  // Trim program down if necessary
                cout << "Trimming program down\n";
                cout << "Size: " << this->sectors.size() << '\n';
                cout << "Max size: " << this->maxSize << '\n';
                for (int i=0; i<this->sectors.size(); i++) {
                    ProgramSector* s = this->sectors[i];
                    if ((s->numLinks == 1) && (i != 0)) {
                        cout << "Found singly-linked sector: (" << s->coord.x << ", " << s->coord.y << ")\n";
                        ProgramSector* connectedSector = s->links[0];
                        // Unlink the sectors
                        for (int j=0; j<connectedSector->links.size(); j++) {
                            cout << "connectedSector: (" << connectedSector->coord.x << ", " << connectedSector->coord.y << ")\n";
                            if (connectedSector->links[j]->coord == s->coord) {
                                connectedSector->links.erase(connectedSector->links.begin() + j);
                                connectedSector->numLinks--;
                                cout << connectedSector->numLinks << '\n';
                                break;
                            }
                        }
                        this->sectors.erase(this->sectors.begin() + i); // Delete the sector from the program
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
    vector<ProgramSector*>::iterator itr = sectors.begin();
    advance(itr, pos);
    ProgramSector* newSector = new ProgramSector(coord, sectors[pos]);
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
