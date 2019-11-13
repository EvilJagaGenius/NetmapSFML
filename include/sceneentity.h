#ifndef SCENEENTITY_H
#define SCENEENTITY_H

#include <fstream>

// AAUGH

#include "toolbox.h"
#include "animator.h"

using namespace std;

class SceneEntity
{
    public:
        string sceneName;
        string filename;
        string type;
        string target;
        sf::Texture sheet;
        sf::Sprite sprite;
        sf::Vector2i coord;
        Animator animator;

        double animTimer;
        double loopTime;

        SceneEntity(string sceneName, string filename);
        virtual ~SceneEntity();
        void loadEntity();
        void frameTick();

    protected:

    private:
};

#endif // SCENEENTITY_H
