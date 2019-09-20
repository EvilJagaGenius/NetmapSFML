#ifndef SCENEENTITY_H
#define SCENEENTITY_H

#include <fstream>

// AAUGH

#include "toolbox.h"
#include "animation.h"

using namespace std;

class SceneEntity
{
    public:
        string sceneName;
        string filename;
        sf::Texture sheet;
        sf::Sprite sprite;
        sf::Vector2i coord;

        SceneEntity(string sceneName, string filename);
        virtual ~SceneEntity();
        void loadEntity();
        // Maybe add frameTick()?

    protected:

    private:
};

#endif // SCENEENTITY_H
