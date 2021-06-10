#include "shopinputbox.h"

ShopInputBox::ShopInputBox() {
    // I forget how to write these things
    this->done = false;
    this->focus = "";

    this->mousePos = sf::Vector2i(-1, -1);

    this->topLeft = sf::Vector2i(32, 32);
    this->rect = sf::Rect<int>(this->topLeft.x, this->topLeft.y, 200, 200);
    this->boxGraphic = sf::RectangleShape(sf::Vector2<float>(this->rect.width, this->rect.height));
    this->boxGraphic.setPosition(this->topLeft.x, this->topLeft.y);
    this->boxGraphic.setFillColor(sf::Color::Black);
    this->boxGraphic.setOutlineThickness(2);
    this->boxGraphic.setOutlineColor(sf::Color::White);
    this->buttonRect = sf::Rect<int>(this->topLeft.x, this->topLeft.y, 200, 14);
    this->buttonGraphic = sf::RectangleShape(sf::Vector2<float>(200, 14));
    this->buttonGraphic.setFillColor(sf::Color::Transparent);
    this->buttonGraphic.setOutlineColor(sf::Color::White);
    this->buttonGraphic.setOutlineThickness(1);
    this->textBox = sf::Text("Do something, Taipu", DEFAULT_FONT, 12);
    this->textBox.setColor(sf::Color::White);
    this->textBox.setOutlineColor(sf::Color::White);

    // Add programs to the list
    this->programList.push_back(new Program("Hack"));
    this->programList.push_back(new Program("Slingshot"));
    this->programList.push_back(new Program("Bug"));
    this->programList.push_back(new Program("DataDr"));
    this->programList.push_back(new Program("BitMan"));
}

ShopInputBox::~ShopInputBox() {
    for (int i=0; i<this->programList.size(); i++) {
        delete this->programList[i];
        this->programList[i] = nullptr;
    }
}

void ShopInputBox::setMousePos(sf::Vector2i mousePos) {
    this->mousePos = mousePos;
}
void ShopInputBox::render(sf::RenderWindow* window, Netmap_Playable* playable) {
    this->highlightedBox = -1;
    window->draw(this->boxGraphic);

    // Draw the program list
    if (playable->localPlayer != nullptr) {
        this->textBox.setPosition(this->topLeft.x, this->topLeft.y);
        this->textBox.setColor(sf::Color::White);
        this->textBox.setString("CREDITS:  $" + to_string(playable->localPlayer->credits));
        window->draw(this->textBox);

        for (int i=0; i<this->programList.size(); i++) {
            int amtOfProgram = 0;
            if (playable->localPlayer->programs.count(this->programList[i]->uploadName) > 0) {
                amtOfProgram = playable->localPlayer->programs[this->programList[i]->uploadName];
            }
            this->textBox.setPosition(this->topLeft.x, this->topLeft.y+(i+1)*14);
            this->textBox.setColor(this->programList[i]->color);
            this->textBox.setString(this->programList[i]->uploadName
                                    + "  " + to_string(amtOfProgram)
                                    + "  $" + to_string(this->programList[i]->cost));
            this->buttonRect.left = this->topLeft.x;
            this->buttonRect.top = this->topLeft.y + (i+1)*14;
            this->buttonGraphic.setPosition(this->topLeft.x, this->topLeft.y+(i+1)*14);
            if (this->buttonRect.contains(mousePos.x, mousePos.y)) {
                this->buttonGraphic.setOutlineThickness(1);
                this->highlightedBox = i;
            } else {
                this->buttonGraphic.setOutlineThickness(0);
            }
            window->draw(this->buttonGraphic);
            window->draw(this->textBox);
        }
    }
}
void ShopInputBox::takeInput(sf::Event event, Netmap_Playable* playable) {
    // We need design the screen better.  Ackpth
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {  // If the player has clicked
            if (this->highlightedBox != -1) {
                // We can set the focus here, provide useful info in DataBattlePlayer, etc
                playable->localPlayer->cmdQueue.push("buy:" + this->programList[this->highlightedBox]->uploadName);
            }
        }
    }
    // Need a way to close the window
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape) {
            this->done = true;
        }
    }
}
string ShopInputBox::getFocus() {
    return this->focus;
}
