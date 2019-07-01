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

    this->programListIndex = 0;

    this->focusType = '0';
    this->focusCoord = *(new sf::Vector2<int>(-1, -1));
    this->focusTextBox = *(new sf::Text("", DEFAULT_FONT, 12));
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

void HUD::takeInput(sf::Event event, Netmap_Playable* playable) {
    cout << "Called HUD/Netmap_Playable version of takeInput\n";
}

void HUD::takeInput(sf::Event event, DataBattle* playable) {
    if (event.type == sf::Event::MouseButtonPressed) {
        if (this->open) {
            if (this->borderRect.contains(mousePos)) {  // Check the border button
                // Close the HUD
                this->open = false;
                this->panelSprite.setPosition(1008, 0);
            }
        } else {
            if (this->borderRect.contains(this->mousePos) && (event.type == sf::Event::MouseButtonPressed)) {
                // Open the HUD
                this->open = true;
                this->panelSprite.setPosition(576, 0);
            }
        }

        // Calculate which tile coordinate, if any, we'd be pointing to
        if (this->mousePos.x < WY) {
            if ((this->mousePos.x % (TILE_SIZE+GAP_SIZE) <= TILE_SIZE) && (this->mousePos.y % (TILE_SIZE+GAP_SIZE) <= TILE_SIZE)) {  // If we're on a tile
                int tileX = this->mousePos.x / (TILE_SIZE+GAP_SIZE);
                int tileY = this->mousePos.y / (TILE_SIZE+GAP_SIZE);
                sf::Vector2<int> tileCoord(tileX, tileY);
                // Uploads
                if (playable->phase == 'u') {
                    for (int i=0; i<playable->uploads.size(); i++) {
                        sf::Vector2<int> coord = playable->uploads[i];
                        if (tileCoord == coord) {
                            playable->selectedUpload = i;
                            // Set an upload as the focus
                            this->focusType = 'u';
                            this->subFocus = i;
                            this->focusCoord.x = tileX;
                            this->focusCoord.y = tileY;
                            break;
                        }
                    }
                    // Defenders
                    for (pair<string, Program*> p : playable->defenders) {
                        for (ProgramSector sector : p.second->sectors) {
                            if (sector.coord == tileCoord) {
                                this->focusType = 'p';
                                this->subFocus = -1;
                                this->focusProgram = p.second;
                                this->focusCoord.x = tileX;
                                this->focusCoord.y = tileY;
                            }
                        }
                    }

                } else {
                    // Do something, Taipu
                }
            }
        } else {  // If we're pointing somewhere on the HUD itself
            // See if we clicked a program button
            int i=0;
            int currentIndex = 0;
            sf::Rect<int> programButtonRect(592, 0, 448, 16);

            for (pair<string, int> p : this->player->programs) {
                if (currentIndex >= this->programListIndex) {
                    programButtonRect.top = i*16;
                    if (programButtonRect.contains(this->mousePos)) {
                        if ((playable->phase == 'u') && (playable->selectedUpload != -1) && (p.second > 0)) {
                            // Upload the program to the grid
                            // Check to see if a program is in the selected spot
                            for (int j=0; j<playable->friendlies.size(); j++) {
                                Program* program = playable->friendlies[j];
                                if (playable->uploads[playable->selectedUpload] == program->sectors[0].coord) {
                                    this->player->programs[program->name]++;   // Add that program to the inventory
                                    playable->friendlies.erase(playable->friendlies.begin()+j);  // Remove from the databattle
                                    playable->friendliesLoaded--;
                                    break;
                                }
                            }
                            Program* newProgram = new Program(p.first);
                            newProgram->move(playable->uploads[playable->selectedUpload], true);
                            playable->friendlies.push_back(newProgram);  // Add to databattle
                            playable->friendliesLoaded++;
                            this->player->programs[p.first]--;  // Remove one from the inventory

                            this->focusProgram = newProgram;
                            this->focusCoord = newProgram->sectors[0].coord;
                            this->focusType = 'p';
                            break;
                        }
                    }
                    i++;
                }
                currentIndex++;
            }

            // DB Initialize
            programButtonRect.top = WY-32;
            if (programButtonRect.contains(this->mousePos) && playable->phase == 'u') {
                if (playable->friendliesLoaded > 0) {
                    playable->switchTurns(this);
                }
            }

            // Disconnect
            programButtonRect.top = WY-16;
            if (programButtonRect.contains(this->mousePos)) {
                cout << "Disconnect\n";
            }
        }
    }
}

void HUD::render(sf::RenderWindow* window, DataBattle* playable) {
    window->draw(this->panelSprite);  // Draw the transparent panel first
    // Draw the contents

    this->contentTexture.clear(sf::Color::Transparent);

    this->contentTexture.draw(this->borderSprite);

    // Draw program names
    int i=0;
    int currentIndex = 0;
    sf::RectangleShape programButton(sf::Vector2<float>(448, 16));
    sf::Rect<int> programButtonRect(592, 0, 448, 16);
    programButton.setFillColor(sf::Color::Transparent);
    programButton.setOutlineColor(sf::Color::White);
    programButton.setOutlineThickness(1);

    for (pair<string, int> p : this->player->programs) {
        if (currentIndex >= this->programListIndex) {
            // Do something, Taipu
            renderText(&(this->contentTexture), PROGRAM_DB[p.first]->screenName + " x" + to_string(p.second), sf::Rect<int>(16, i*16, 448, 16), DEFAULT_FONT, 12, PROGRAM_DB[p.first]->color);
            programButtonRect.top = i*16;
            programButton.setPosition(16, programButtonRect.top);
            if (programButtonRect.contains(this->mousePos)) {
                this->contentTexture.draw(programButton);
            }
            i++;
        }
        currentIndex++;
    }

    // Draw focus and associated data
    if (this->focusType != '0') {
        this->focusTextBox.setStyle(sf::Text::Regular);
        this->focusTextBox.setColor(sf::Color::White);
        this->focusTextBox.setString("@" + getByteCoord(this->focusCoord));
        this->focusTextBox.setPosition(20 + TILE_SIZE, 170);
        this->contentTexture.draw(this->focusTextBox);
        if (this->focusType == 'u') { // Upload zone
            // Draw the sprite
            this->focusSprite.setTexture(GRID_SHEET);  // Maybe I should combine the program sheets and grid sheets at some point
            this->focusSprite.setTextureRect(sf::Rect<int>(0, 2*TILE_SIZE, TILE_SIZE, TILE_SIZE));
            this->focusSprite.setPosition(16, 160);
            this->contentTexture.draw(this->focusSprite);

            // Draw name
            this->focusTextBox.setPosition(20 + TILE_SIZE, 160);
            this->focusTextBox.setString("UPLOAD ZONE " + to_string(this->subFocus));
            this->contentTexture.draw(this->focusTextBox);

            // Draw description
            this->focusTextBox.setPosition(220, 160);
            this->focusTextBox.setString("Upload programs to the memory grid");
            this->contentTexture.draw(this->focusTextBox);

        } else if (this->focusType == 'p') { // Program
            // Draw the sprite
            this->focusSprite.setTexture(PROGRAM_SHEET);
            this->focusSprite.setTextureRect(sf::Rect<int>(this->focusProgram->spriteCoord.x*TILE_SIZE, this->focusProgram->spriteCoord.y*TILE_SIZE, TILE_SIZE, TILE_SIZE));
            this->focusSprite.setPosition(16, 160);
            this->contentTexture.draw(this->focusSprite);

            // Draw name
            this->focusTextBox.setPosition(20 + TILE_SIZE, 160);
            this->focusTextBox.setColor(this->focusProgram->color);
            this->focusTextBox.setString(this->focusProgram->screenName);
            this->contentTexture.draw(this->focusTextBox);

            // Draw description
            renderText(&this->contentTexture, this->focusProgram->description, sf::Rect<int>(220, 160, 228, 100), DEFAULT_FONT, 12, sf::Color::White);
        }
    }

    // Draw DB Initialize, Disconnect buttons
    programButtonRect.top = WY-32;
    programButton.setPosition(16, programButtonRect.top);
    if (playable->phase == 'u') {
        if (playable->friendliesLoaded > 0) {
            renderText(&(this->contentTexture), "DATABATTLE INITIALIZE", sf::Rect<int>(16, programButtonRect.top, 448, 16), DEFAULT_FONT, 12, sf::Color::Green);
        } else {
            renderText(&(this->contentTexture), "UPLOAD PROGRAMS", sf::Rect<int>(16, programButtonRect.top, 448, 16), DEFAULT_FONT, 12, sf::Color::Red);
        }
    } else {
        renderText(&(this->contentTexture), "DATABATTLE IN PROGRESS", sf::Rect<int>(16, programButtonRect.top, 448, 16), DEFAULT_FONT, 12, sf::Color::White);
    }
    if (programButtonRect.contains(this->mousePos)) {
        this->contentTexture.draw(programButton);
    }

    // Disconnect button
    programButtonRect.top = WY-16;
    programButton.setPosition(16, programButtonRect.top);
    renderText(&(this->contentTexture), "DISCONNECT", sf::Rect<int>(16, programButtonRect.top, 448, 16), DEFAULT_FONT, 12, sf::Color::White);
    if (programButtonRect.contains(this->mousePos)) {
        this->contentTexture.draw(programButton);
    }

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

void HUD::setPlayer(Player* p) {
    this->player = p;
}
void HUD::setFocus(Program* focus) {
    this->focusType = 'p';
    this->focusProgram = focus;
    this->focusCoord = focus->sectors[0].coord;
}
