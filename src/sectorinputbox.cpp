#include "sectorinputbox.h"

SectorInputBox::SectorInputBox() {
    this->boxGraphic = sf::RectangleShape(sf::Vector2<float>(100, 100));
    this->boxGraphic.setFillColor(sf::Color::Black);
    this->boxGraphic.setPosition(WY, 200);

    this->textBox = sf::Text("Select a coordinate", DEFAULT_FONT, 12);
    this->textBox.setColor(sf::Color::White);
    this->textBox.setPosition(WY, 200);

    this->done = false;
}

SectorInputBox::~SectorInputBox() {}
void SectorInputBox::render(sf::RenderWindow* window, Netmap_Playable* playable) {
    window->draw(this->boxGraphic);
    window->draw(this->textBox);
}
void SectorInputBox::setMousePos(sf::Vector2i mousePos) {
    this->mousePos = mousePos;
}
void SectorInputBox::takeInput(sf::Event event, Netmap_Playable* playable) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        if ((this->mousePos.x <= WY) && (this->mousePos.x % (TILE_SIZE + GAP_SIZE) <= TILE_SIZE) && (this->mousePos.y % (TILE_SIZE + GAP_SIZE) <= TILE_SIZE)) {
            // Do something, Taipu
            coord.x = mousePos.x / (TILE_SIZE + GAP_SIZE);
            coord.y = mousePos.y / (TILE_SIZE + GAP_SIZE);
            this->done = true;
            cout << "SectorInputBox->done\n";
        }
    }
}

string SectorInputBox::getFocus() {
    return "";
}
int SectorInputBox::getSubFocus() {
    return -1;
}
