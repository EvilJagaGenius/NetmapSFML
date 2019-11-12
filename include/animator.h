#ifndef ANIMATOR_H
#define ANIMATOR_H

// Clone of the Thor library's Animator class

#include <unordered_map>
#include <SFML/Graphics.hpp>

#include "animation.h"

using namespace std;

class Animator
{
    public:
        unordered_map<string, Animation> animations;
        float progress;
        bool loop;
        float loopTime;
        Animation currentAnim;

        Animator();
        virtual ~Animator();
        void addAnimation(string id, Animation anim);
        void playAnimation(string id, bool loop=false);  // This function switches animations
        //void stopAnimation();
        //bool isPlayingAnimation();
        //string getPlayingAnimation();
        void update(sf::Time dt);  // Use this to keep track of a sprite's progress through the animation
        void update(float progress);
        //void update(float progress);
        void animate(sf::Sprite* target, float progress);  // Why do we have the progress parameter if we're supposed to be keeping track of that..?

    protected:

    private:
};

#endif // ANIMATOR_H
