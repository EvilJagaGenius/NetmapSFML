#include "textinputbox.h"

TextInputBox::TextInputBox(string prompt) {
    this->done = false;
    this->topLeft = sf::Vector2i(400, 400);
    this->rect = sf::Rect<int>(this->topLeft.x, this->topLeft.y, 400, 50);
    this->boxGraphic = sf::RectangleShape(sf::Vector2<float>(this->rect.width, this->rect.height));
    this->boxGraphic.setPosition(this->topLeft.x, this->topLeft.y);
    this->boxGraphic.setFillColor(sf::Color::Black);
    this->boxGraphic.setOutlineThickness(2.0);
    this->boxGraphic.setOutlineColor(sf::Color::White);
    this->textBox = sf::Text("", DEFAULT_FONT, 12);

    this->prompt = prompt;
    this->input = "";
}

TextInputBox::~TextInputBox() {
    //dtor
}

void TextInputBox::setMousePos(sf::Vector2i mousePos) {
    this->mousePos = mousePos;
}

void TextInputBox::render(sf::RenderWindow* window, Netmap_Playable* playable) {
    window->draw(this->boxGraphic);
    // Draw prompt
    this->textBox.setPosition(this->topLeft.x, this->topLeft.y);
    this->textBox.setColor(sf::Color::White);
    this->textBox.setString(this->prompt);
    window->draw(this->textBox);
    // Draw input
    this->textBox.setPosition(this->topLeft.x, this->topLeft.y+14);
    this->textBox.setString(this->input);
    window->draw(this->textBox);
}

void TextInputBox::takeInput(sf::Event event, Netmap_Playable* playable) {
    //cout << "Called TextInputBox::takeInput()\n";
    if (event.type == sf::Event::TextEntered) {
        int value = event.text.unicode;
        char letter = event.text.unicode;
        if (letter == 13) {  // 13 is Enter
            this->done = true;
        } else if (letter == 8) {  // 8 is backspace
            if (this->input.size() > 0) {
                this->input.pop_back();
            }
        } else if (letter == 27) {  // 27 is Escape
            this->input = "";
            this->done = true;
        } else {
            cout << "As int: " << value << '\n';
            cout << "As char: " << letter << '\n';
            this->input += letter;
        }
        // AAUGH
    }
}

void TextInputBox::setFocus(string focus) {}  // I don't think we need this
void TextInputBox::setSubFocus(int subFocus) {}  // Same here
string TextInputBox::getFocus() {
    return this->input;
}
