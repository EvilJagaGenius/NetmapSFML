#include "programaction.h"

// ProgramAction
ProgramAction::ProgramAction() {
    this->numOfTargets = 1;  // 1 target by default
}
ProgramAction::~ProgramAction() {}
void ProgramAction::load(string filename) {
    // Do something, Taipu
    this->filename = filename;
    cout << "Loading action " << filename << '\n';
    ifstream textFile;
    textFile.open(filename);
    string line;
    char loading = '0';

    while (getline(textFile, line)) {
        if (startsWith(line, "//")) {  // Ignore comments
            continue;
        } else if (startsWith(line, "name:")) {
            this->actionName = splitString(line, ':')[1];
        } else if (startsWith(line, "desc:")) {
            this->description = splitString(line, ':')[1];
        } else if (startsWith(line, "targets:")) {
            this->numOfTargets = stoi(splitString(line, ':')[1]);
        } else if (startsWith(line, "range:")) {
            this->range = stoi(splitString(line, ':')[1]);
        } else if (startsWith(line, "sprite:")) {
            this->targetSprite = stoi(splitString(line, ':')[1]);
        } else if (startsWith(line, "COMMANDS")) {
            loading = 'c';
            continue;
        } else if (startsWith(line, "PREREQS")) {
            loading = 'p';
            continue;
        }
        if (loading == 'c') {  // Commands
            if (startsWith(line, "END_COMMANDS")) {
                loading = '0';
                continue;
            } else {
                this->commands.push_back(line);
            }
        }
        if (loading == 'p') {  // Prereqs
            if (startsWith(line, "END_PREREQS")) {
                loading = '0';
                continue;
            } else {
                this->prereqs.push_back(line);
            }
        }
    }
    textFile.close();
}

/*void ProgramAction::use(Netmap_Playable* db, DataBattlePiece* source, vector<sf::Vector2i> targets) {
    // We don't need this function anymore, stuff is handled by the DataBattle now
    cout << "Called ProgramAction::use()\n";
    for (string command : this->commands) {
        cout << command << '\n';
    }
}*/
vector<sf::Vector2i> ProgramAction::getAimArea(sf::Vector2i origin, int targetNum) {
    return getRadius(this->range, origin, false);
}
bool ProgramAction::checkPrereqs(DataBattlePiece* p) {
    // Do we check this someplace else?  DataBattle, DataBattlePiece?
    for (string condition : this->prereqs) {
        if (startsWith(condition, "size:")) {  // Size
            if (p->size <= stoi(condition.substr(5))) {
                return false;
            }
        } else if (startsWith(condition, "move:")) {  // Movement
            if (p->currentMove <= stoi(condition.substr(5))) {
                return false;
            }
        }
    }
    return true;  // Assuming we passed all the checks
}
