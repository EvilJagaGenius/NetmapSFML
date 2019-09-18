#include "sceneentity.h"

SceneEntity::SceneEntity()
{
    //ctor
}

SceneEntity::~SceneEntity()
{
    //dtor
}

void SceneEntity::loadEntity() {
    // Do something, Taipu
    ifstream textFile;
    textFile.open("Data\\Scenes\\" + this->sceneName + "\\Entities\\" + this->filename);
    string line;

    while(getline(textFile, line)) {
        vector<string> splitLine = splitString(line, ':');
        if (startsWith(line, "type:")) {
            // ...I'm not sure what to do with this information yet
        }
        if (startsWith(line, "sheet:")) {
            // Create the sprite
            this->sheet = imgLoad("Data\\Scenes\\" + this->sceneName + "\\Graphics\\" + this->filename);
            this->sprite.setTexture(this->sheet);
        }
        if (startsWith(line, "anim:")) {
            // We need some way to manage animations
            // Consider cloning the Thor Animator class
            // GEE, IT'D BE REALLY NICE IF THAT LIBRARY ACTUALLY WORKED, WOULDN'T IT?!
        }
        if (startsWith(line, "coord:")) {
            this->coord.x = stoi(splitLine[1]);
            this->coord.y = stoi(splitLine[2]);
        }
    }
}
