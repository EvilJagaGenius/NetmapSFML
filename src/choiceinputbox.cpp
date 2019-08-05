#include "choiceinputbox.h"

ChoiceInputBox::ChoiceInputBox(sf::Vector2i topLeft, vector<string> options, vector<bool> usable)
{
    this->topLeft = topLeft;
    this->options = options;
    this->usable = usable;
    this->contentTexture->create(100, options.size()*14);
}

ChoiceInputBox::~ChoiceInputBox()
{
    //dtor
}

void ChoiceInputBox::setMousePos(sf::Vector2i mousePos) {
    this->mousePos = mousePos;
}

void ChoiceInputBox::render(sf::RenderWindow* window, Netmap_Playable* playable) {
    this->contentTexture->clear();


    sf::Sprite contentSprite = sf::Sprite(this->contentTexture->getTexture());
    contentSprite.setPosition(this->topLeft);
    window->draw(contentSprite);
}

void ChoiceInputBox::takeInput(sf::Event event, Netmap_Playable* playable) {
    // Do more things, Taipu
}

void ChoiceInputBox::setFocus(string focus) {}
void ChoiceInputBox::setSubFocus(int subFocus) {}
string getFocus() {
    return this->focus;
}
