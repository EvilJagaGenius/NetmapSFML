#ifndef NPC_H
#define NPC_H

#include <fstream>
#include <SFML/Graphics.hpp>

#include "netmap_playable.h"

// An NPC is a box to hold a conversation

using namespace std;

class NPC: public Netmap_Playable
{
    public:
        string filename;
        sf::Texture bkgTexture;
        sf::Sprite bkgSprite;

        // We're gonna need some code for animating the characters too

        unordered_map<string, vector<string>> text;
        string topic;
        string startTopic;
        int part;
        int startPart;
        string currentText;

        bool paused;
        float pauseCounter;

        sf::Font font;
        sf::Color textColor;

        int spaceLength;
        int fontHeight;
        int xLimit;
        int yLimit;

        bool stopAnimating;
        int currentWordIndex;
        string currentWord;
        int currentLetterIndex;
        char currentLetter;
        int letterX;
        int letterY;
        sf::Vector2i letterSize;

        sf::RenderTexture textSurface;
        sf::Text letterSurface;

        NPC(string filename);
        virtual ~NPC();
        void load();
        void render(sf::RenderWindow* window);
        string play(sf::RenderWindow* window);
        void advance();
        void resetAnimationStuff();
        void checkForFlags();
        void clearSurface();
        void goTo(string topic, int part);
        void resetConversation();
        void tick();

    protected:

    private:
};

#endif // NPC_H
