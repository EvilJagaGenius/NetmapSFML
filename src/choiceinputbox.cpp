#include "choiceinputbox.h"

ChoiceInputBox::ChoiceInputBox(sf::Vector2i topLeft, vector<string> options, vector<bool> usable, int maxSize)
{
    this->topLeft = sf::Vector2<float>(topLeft.x, topLeft.y);
    this->options = options;
    this->usable = usable;
    this->maxSize = maxSize;
    this->optionIndex = 0;
    this->subFocus = -1;
    if (options.size() <= this->maxSize) {
        this->maxSize = options.size();
    }

    this->buttonRect = sf::Rect<int>(this->topLeft.x, this->topLeft.y, 100, 14);
    this->buttonGraphic = sf::RectangleShape(sf::Vector2<float>(100, 14));
    this->buttonGraphic.setFillColor(sf::Color::Transparent);
    this->buttonGraphic.setOutlineColor(sf::Color::White);
    this->buttonGraphic.setOutlineThickness(1);
    this->boxGraphic = sf::RectangleShape(sf::Vector2<float>(100, 14*this->maxSize));
    this->boxGraphic.setPosition(this->topLeft);
    this->boxGraphic.setFillColor(sf::Color::Black);
    this->textBox = sf::Text("", DEFAULT_FONT, 12);
    this->done = false;
}

ChoiceInputBox::~ChoiceInputBox()
{
    //dtor
}

void ChoiceInputBox::setMousePos(sf::Vector2i mousePos) {
    this->mousePos = mousePos;
}

void ChoiceInputBox::render(sf::RenderWindow* window, Netmap_Playable* playable) {
    window->draw(this->boxGraphic);
    for (int i=0; i<this->maxSize; i++) {
        this->buttonRect.top = (this->topLeft.y + i*14);
        this->textBox.setString(this->options[this->optionIndex + i]);
        this->textBox.setPosition(this->buttonRect.left, this->buttonRect.top);
        if (usable[optionIndex + i]) {
            this->textBox.setColor(sf::Color::White);
        } else {
            this->textBox.setColor(sf::Color::Red);
        }
        window->draw(this->textBox);
        if (buttonRect.contains(this->mousePos) && usable[optionIndex+i]) {
            this->subFocus = optionIndex + i;
            this->focus = this->options[optionIndex + i];
            this->buttonGraphic.setPosition(this->buttonRect.left, this->buttonRect.top);
            window->draw(this->buttonGraphic);
        }
    }
}

void ChoiceInputBox::takeInput(sf::Event event, Netmap_Playable* playable) {
    if (event.type == sf::Event::MouseButtonPressed) {
        this->done = true;
    } else if (event.type == sf::Event::MouseWheelScrolled) {
        if (event.mouseWheelScroll.delta > 0) {  // If we scrolled up
            if (this->optionIndex > 0) {
                this->optionIndex--;
            }
        } else if (event.mouseWheelScroll.delta < 0) {  // If we scrolled down
            if ((this->optionIndex + this->maxSize) < this->options.size()) {
                this->optionIndex++;
            }
        }
    }
}

void ChoiceInputBox::setFocus(string focus) {}
void ChoiceInputBox::setSubFocus(int subFocus) {}
string ChoiceInputBox::getFocus() {
    //return "";
    return this->focus;
}
int ChoiceInputBox::getSubFocus() {
    return this->subFocus;
}
