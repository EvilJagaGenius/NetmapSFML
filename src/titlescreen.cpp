#include "titlescreen.h"

TitleScreen::TitleScreen() {
    this->button = sf::Rect<int>(0,0,100,12);
    this->buttonGraphic = sf::RectangleShape(sf::Vector2<float>(100, 14));
    this->buttonGraphic.setFillColor(sf::Color::Transparent);
    this->buttonGraphic.setOutlineColor(sf::Color::White);
    this->buttonGraphic.setOutlineThickness(1);
    this->buttonGraphic.setPosition(this->button.left, this->button.top);
    this->textBox = sf::Text("", DEFAULT_FONT, 12);
    this->textBox.setColor(sf::Color::White);
    this->textBox.setPosition(this->button.left, this->button.top);

    this->currentInputBox = nullptr;
}

TitleScreen::~TitleScreen() {
    if (this->currentInputBox != nullptr) {
        delete this->currentInputBox;
        this->currentInputBox = nullptr;
    }
}

void TitleScreen::setHUD(InputBox* hud) {}
void TitleScreen::setPlayer(Player* player) {}
void TitleScreen::render(sf::RenderWindow* window) {
    // Draw background image, once we have one
    // Start Game
    this->button.top = 0;
    this->textBox.setString("Start Game");
    this->textBox.setPosition(this->button.left, this->button.top);
    window->draw(this->textBox);
    if (this->button.contains(this->mousePos)) {
        this->buttonGraphic.setPosition(this->button.left, this->button.top);
        window->draw(this->buttonGraphic);
    }

    // Play DB
    this->button.top = 14;
    this->textBox.setString("Play DataBattle");
    this->textBox.setPosition(this->button.left, this->button.top);
    window->draw(this->textBox);
    if (this->button.contains(this->mousePos)) {
        this->buttonGraphic.setPosition(this->button.left, this->button.top);
        window->draw(this->buttonGraphic);
    }

    // Editor
    this->button.top = 28;
    this->textBox.setString("Open DB Editor");
    this->textBox.setPosition(this->button.left, this->button.top);
    window->draw(this->textBox);
    if (this->button.contains(this->mousePos)) {
        this->buttonGraphic.setPosition(this->button.left, this->button.top);
        window->draw(this->buttonGraphic);
    }

    // Netplay
    this->button.top = 42;
    this->textBox.setString("Netplay");
    this->textBox.setPosition(this->button.left, this->button.top);
    window->draw(this->textBox);
    if (this->button.contains(this->mousePos)) {
        this->buttonGraphic.setPosition(this->button.left, this->button.top);
        window->draw(this->buttonGraphic);
    }

    // Quit
    this->button.top = 56;
    this->textBox.setString("Quit");
    this->textBox.setPosition(this->button.left, this->button.top);
    window->draw(this->textBox);
    if (this->button.contains(this->mousePos)) {
        this->buttonGraphic.setPosition(this->button.left, this->button.top);
        window->draw(this->buttonGraphic);
    }
    if (this->currentInputBox != nullptr) {
        this->currentInputBox->render(window, this);
    }
}
string TitleScreen::play(sf::RenderWindow* window) {
    bool leftClicked = false;
    sf::Event event;
    char textBoxType = '0';

    while (window->isOpen()) {
        window->clear();
        this->mousePos = sf::Mouse::getPosition(*window);
        leftClicked = false;

        while (window->pollEvent(event)) {
            if (this->currentInputBox != nullptr) {
                this->currentInputBox->takeInput(event, this);
            }
            if (event.type == sf::Event::Closed) {
                window->close();
            } else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    leftClicked = true;
                }
            }
        }
        if (leftClicked) {
            // Start Game
            button.top = 0;
            if (button.contains(mousePos)) {
                return "scene:RNA_1";
            }
            // Play DB
            button.top = 14;
            if (button.contains(mousePos)) {
                this->currentInputBox = new TextInputBox("Enter the name of the DataBattle:");
                textBoxType = 'f';
            }
            // Editor
            button.top = 28;
            if (button.contains(mousePos)) {
                return "editor:";
            }
            button.top = 42;
            // Netplay
            if (button.contains(mousePos)) {
                this->currentInputBox = new TextInputBox("Enter IP address to connect to:");
                textBoxType = 'n';
            }
            // Quit
            button.top = 56;
            if (button.contains(mousePos)) {
                return "quit:";
            }

        }
        if (this->currentInputBox != nullptr) {
            if (textBoxType == 'f') {  // Playing DBs from a filename
                if (this->currentInputBox->done) {
                    string filename = this->currentInputBox->getFocus();
                    delete this->currentInputBox;
                    this->currentInputBox = nullptr;
                    textBoxType = '0';
                    // See if the DB exists (Code copied from Stack Overflow)
                    if (filename.size() > 0) {
                        if (auto dir = opendir("Data\\DataBattles")) {
                            while (auto f = readdir(dir)) {
                                if (!f->d_name || f->d_name[0] == '.') {
                                    continue; // Skip everything that starts with a dot
                                } else if (startsWith(f->d_name, filename) && endsWith(f->d_name, ".txt")) {
                                    cout << "Valid DB\n";
                                    //return ("db:" + filename);
                                    return "quit:";
                                }
                            }
                            closedir(dir);
                        }
                    }
                }
            } else if (textBoxType == 'n') {  // Netplay
                if (this->currentInputBox->done) {
                    string ipAddress = this->currentInputBox->getFocus();
                    delete this->currentInputBox;
                    this->currentInputBox = nullptr;
                    textBoxType = '0';
                    return ("netlobby:" + ipAddress + ":9900");
                }
            }
        }
        this->render(window);
        window->display();
    }
    return "quit:";
}
