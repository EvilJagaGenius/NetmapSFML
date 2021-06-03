#include "shopinputbox.h"

ShopInputBox::ShopInputBox() {
    // I forget how to write these things
    done = false;

    this->mousePos = sf::Vector2i(-1, -1);

    this->topLeft = sf::Vector2i(32, 32);
    this->rect = sf::Rect<int>(this->topLeft.x, this->topLeft.y, 200, 200);
    this->boxGraphic = sf::RectangleShape(sf::Vector2<float>(this->rect.width, this->rect.height));
    this->boxGraphic.setPosition(this->topLeft.x, this->topLeft.y);
    this->boxGraphic.setFillColor(sf::Color::Black);
    this->boxGraphic.setOutlineThickness(2.0);
    this->boxGraphic.setOutlineColor(sf::Color::White);
    this->textBox = sf::Text("Do something, Taipu", DEFAULT_FONT, 12);
    this->textBox.setColor(sf::Color::White);

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
    window->draw(this->boxGraphic);
    if (playable->localPlayer != nullptr) {
        for (int i=0; i<this->programList.size(); i++) {
            int amtOfProgram = 0;
            if (playable->localPlayer->programs.count(this->programList[i]->uploadName) > 0) {
                amtOfProgram = playable->localPlayer->programs[this->programList[i]->uploadName];
            }
            this->textBox.setPosition(this->topLeft.x, this->topLeft.y+(i*14));
            this->textBox.setColor(this->programList[i]->color);
            this->textBox.setString(this->programList[i]->uploadName
                                    + "  " + to_string(amtOfProgram)
                                    + "  $" + to_string(this->programList[i]->cost));
            window->draw(this->textBox);
        }
    }
}
void ShopInputBox::takeInput(sf::Event event, Netmap_Playable* playable) {
}
string ShopInputBox::getFocus() {
    return "Do something, Taipu";
}
