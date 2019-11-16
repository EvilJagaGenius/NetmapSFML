#ifndef NPC_H
#define NPC_H

#include <fstream>
#include <string>
#include <SFML/Graphics.hpp>

#include "netmap_playable.h"
#include "choiceinputbox.h" // We might need this
#include "NPCAnim.h"

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
        vector<string> currentSplitText;
        sf::Vector2i mousePos;

        bool paused;
        double pauseCounter;
        double pauseDuration;

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
        sf::Vector2i wordSize;

        bool endConversation;

        sf::RenderTexture textSurface;
        sf::Sprite textSprite;
        sf::Text letterSurface;
        sf::Text wordSurface;
        sf::Sprite currentImage;

        InputBox* dialogBox;
        vector<string> choices;

        // We need something to display all the animatable... bits on screen.
        // Say Gemma and Winter are having a conversation.  We could have animatable bits for both Gemma and Winter, with their own spritesheets.
        // We want to reference them both by strings, but they require too much data to contain in one simple thing... at minimum, they need an sf::Sprite and an Animation.
        // Or could we..?  We define the animations in the NPC's file.  We just need to apply them to a sprite, which isn't that hard.
        // Let's do that and see what happens.

        unordered_map<string, NPCAnim*> characters;
        unordered_map<string, Animation> animations;
        unordered_map<string, sf::Texture> images;

        NPC(string filename);
        virtual ~NPC();
        void load();
        void render(sf::RenderWindow* window);
        string play(sf::RenderWindow* window);
        void advance();
        void advanceWord();
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
