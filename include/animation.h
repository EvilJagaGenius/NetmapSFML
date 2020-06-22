#ifndef ANIMATION_H
#define ANIMATION_H

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

// Mostly copying the Thor FrameAnimation class here.  This means we're doing our animations using actual time increments, not frames.

using namespace std;

class Animation
{
    public:
        vector<float> increments;
        vector<sf::Rect<int> > rects;
        double totalTime;

        Animation();
        virtual ~Animation();

        void addFrame(double duration, sf::Rect<int> textureRect);
        void animate(sf::Sprite* sprite, float progress);

    protected:

    private:
};

#endif // ANIMATION_H
