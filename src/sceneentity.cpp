#include "sceneentity.h"

SceneEntity::SceneEntity(string sceneName, string filename) {
    this->sceneName = sceneName;
    this->filename = filename;
    this->loadEntity();

    this->animator.playAnimation("idle", true);
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
            this->type = splitLine[1];
        }
        if (startsWith(line, "target:")) {
            this->target = splitLine[1] + ":" + splitLine[2];
        }
        if (startsWith(line, "sheet:")) {
            // Create the sprite
            if (!startsWith(splitLine[1], "dummy")) {
                this->sheet = imgLoad("Data\\Scenes\\" + this->sceneName + "\\Graphics\\" + splitLine[1]);
                this->sprite.setTexture(this->sheet);
                this->dummyGraphics = false;
            } else {
                this->dummyGraphics = true;
                this->sprite.setTextureRect(sf::Rect<int>(0, 0, stoi(splitLine[2]), stoi(splitLine[3])));
                this->sprite.setColor(sf::Color::Transparent);
            }
        }
        if (startsWith(line, "add_anim:")) {
            // We need some way to manage animations
            // Consider cloning the Thor Animator class
            // GEE, IT'D BE REALLY NICE IF THAT LIBRARY ACTUALLY WORKED, WOULDN'T IT?!
            string animName = splitLine[1];
            Animation newAnim = animationLoad("Data\\Scenes\\" + this->sceneName + "\\Graphics\\" + splitLine[2]);
            this->animator.addAnimation(animName, newAnim);
        }
        if (startsWith(line, "coord:")) {
            this->coord.x = stoi(splitLine[1]);
            this->coord.y = stoi(splitLine[2]);
        }
    }
}

void SceneEntity::frameTick() {
    if (!this->dummyGraphics) {
        this->animator.update(1.0 / 60.0);  // Change to delta-time!
        this->animator.animate(&this->sprite);
    }
}
