#ifndef ANIMATION_H
#define ANIMATION_H


// Mostly copying the Thor FrameAnimation class here.  This means we're doing our animations using actual time increments, not frames.

class Animation
{
    public:
        vector<float> increments;
        vector<sf::Rect<int>> rects;

        Animation();
        virtual ~Animation();

        void addFrame(float duration, sf::Rect<int> textureRect);
        void animate(sf::Sprite sprite, float progress);

    protected:

    private:
};

#endif // ANIMATION_H
