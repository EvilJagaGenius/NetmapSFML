#ifndef NPCANIM_H
#define NPCANIM_H

#include <unordered_map>
#include <SFML/Graphics.hpp>

#include "animation.h"
#include "toolbox.h"

class NPCAnim
{
    public:
        string sheetFilename;
        sf::Texture spritesheet;  // We might not need this
        sf::Sprite sprite;
        unordered_map<string, Animation*> animations;
        Animation* currentAnimation;
        Animation* endAnimation;
        bool loop;
        bool visible;
        double progress;

        NPCAnim();
        NPCAnim(string sheetFilename);
        virtual ~NPCAnim();
        void load();
        void changeAnimation(string newAnimation);
        void tick();
        void addAnimation(string animName, Animation* newAnimation);

    protected:

    private:
};

#endif // NPCANIM_H
