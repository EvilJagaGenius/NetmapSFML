#ifndef SCENELAYER_H
#define SCENELAYER_H

#include "sceneentity.h"

using namespace std;

class SceneLayer
{
    public:
        string sceneName;
        string bkgFilename;
        int scrollRateX;
        int scrollRateY;
        //sf::Rect<int> rect;  // What do we use this for?
        vector<SceneEntity> entities;
        sf::Texture bkgTexture;
        sf::Sprite bkgSprite;  // I think this works?

        SceneLayer(string bkgFilename);
        virtual ~SceneLayer();
        void render(sf::RenderWindow* window);
        // Maybe add a frameTick() method?

    protected:

    private:
};

#endif // SCENELAYER_H
