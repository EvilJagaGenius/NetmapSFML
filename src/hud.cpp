#include "hud.h"

HUD::HUD()
{
    this->open = true;
    this->panelTexture.loadFromFile("Data\\Sprites\\HudPanel.png");
    this->panelSprite = *(new sf::Sprite(panelTexture));
    this->panelSprite.setPosition(576, 0);
    this->borderTexture.loadFromFile("Data\\Sprites\\HudBar.png");
    this->borderSprite = *(new sf::Sprite(borderTexture));
    this->borderSprite.setTextureRect(*(new sf::Rect<int>(0, 0, 16, 576)));
    this->borderRect = *(new sf::Rect<int>(1008, 0, 16, 576));

    this->contentTexture.create(448, 576);
}

HUD::~HUD()
{
    //dtor
}

void HUD::setMousePos(sf::Vector2i mousePos) {
    this->mousePos = mousePos;
    if (this->open) {
        this->borderRect.left = 576;
        if (this->borderRect.contains(mousePos)) {
            this->borderSprite.setTextureRect(*(new sf::Rect<int>(16, 0, 16, 576)));
        } else {
            this->borderSprite.setTextureRect(*(new sf::Rect<int>(0, 0, 16, 576)));
        }
    } else {  // If the HUD is closed:
        this->borderRect.left = 1008;
        if (this->borderRect.contains(mousePos)) {
            this->borderSprite.setTextureRect(*(new sf::Rect<int>(48, 0, 16, 576)));
        } else {
            this->borderSprite.setTextureRect(*(new sf::Rect<int>(32, 0, 16, 576)));
        }
    }
}

void HUD::takeInput(sf::Event event) {
    if (this->open) {
        if (event.type == sf::Event::MouseButtonPressed) {
            if (this->borderRect.contains(mousePos)) {
                // Close the HUD
                this->open = false;
                this->panelSprite.setPosition(1008, 0);
            }
        }
    } else {
        if (this->borderRect.contains(this->mousePos) && (event.type == sf::Event::MouseButtonPressed)) {
            // Open the HUD
            this->open = true;
            this->panelSprite.setPosition(576, 0);
        }
    }

}

void HUD::render(sf::RenderWindow* window) {
    window->draw(this->panelSprite);  // Draw the transparent panel first
    // Draw the contents
    this->contentTexture.clear(sf::Color::Transparent);

    this->contentTexture.draw(this->borderSprite);
    renderText(&(this->contentTexture), "Hello world!", DEFAULT_FONT, sf::Color::White);

    // Done drawing to the content layer
    this->contentTexture.display();
    this->contentSprite = *(new sf::Sprite(this->contentTexture.getTexture()));

    if (this->open) {
        this->contentSprite.setPosition(576, 0);
    } else {
        this->contentSprite.setPosition(1008, 0);
    }
    window->draw(this->contentSprite);

}
