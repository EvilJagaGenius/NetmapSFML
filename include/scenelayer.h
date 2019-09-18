#ifndef SCENELAYER_H
#define SCENELAYER_H

#include "sceneentity.h"

using namespace std;

class SceneLayer
{
    public:
        string sceneName;
        int scrollRateX;
        int scrollRateY;
        sf::Rect<int> rect;
        vector<SceneEntity> entities;
        sf::Sprite bkg;  // I think this works?

        SceneLayer();
        virtual ~SceneLayer();
        void render(sf::RenderWindow* window);
        // Maybe add a frameTick() method?

    protected:

    private:
};

#endif // SCENELAYER_H
