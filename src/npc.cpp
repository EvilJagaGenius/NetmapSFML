#include "npc.h"

NPC::NPC(string filename) {
    this->filename = filename;
    this->startTopic = 'open';
    this->startPart = 0;
    this->currentText = "";
    this->paused = false;
    this->pauseCounter = 0;
    this->font = DEFAULT_FONT;
    this->textColor = sf::Color::White;

    this->load();
}

NPC::~NPC() {}  // Don't think we need this yet

void NPC::load() {
    ifstream textFile;
    textFile.open("Data\\NPCs\\" + this->filename);
    string line;
    vector<string> splitLine;  // Don't know if we need this yet
    bool loadingConversation = false;

    while (getLine(textFile, line)) {

        // Conversation loading shenanigans
        if (startsWith(line, "LOAD_CONVERSATION")) {
            loadingConversation = true;
        }
        if (loadingConversation) {
            if (startsWith(line, "LOAD_CONVERSATION")) {
                continue;  // Do nothing
            } else if (startsWith(line, "END_CONVERSATION")) {
                loadingConversation = false;
            } else {
                // Load a topic into the conversation
                splitLine = splitString(line, "|");
                string newTopic = splitLine[0];
                splitLine.erase(splitLine.begin());  // Remove the topic marker so it doesn't show up in the conversation text
                this->text.insert({{newTopic, splitLine}});
            }
        }
        // Done with loading conversations
        // We need stuff to load graphics, animations
    }
}

void NPC::render(sf::RenderWindow* window) {
    // Do something, Taipu
}

string NPC::play(sf::RenderWindow* window) {
    while (window->isOpen()) {
        window->clear();

        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window->close();
            }
        }

        this->render(window);
        window->display();
    }

    return "quit:";
}

void NPC::advance() {
    this->part += 1;
    this->currentText = this->text[this->topic][this->part];
    this->resetAnimationStuff();
    this->checkForFlags();

    if (!this->stopAnimating) {  // I forget why these if statements are here...
        this->clearSurface();
    }
}

void NPC::resetAnimationStuff() {

}

void NPC::checkForFlags() {
    // Do something, Taipu
}

void NPC::clearSurface() {

}

void NPC::goTo(string topic, int part) {
    this->topic = topic;
    this->part = part;
    this->currentText = this->text[this->topic][this->part];
    this->checkForFlags();

    if (!this->stopAnimating) {
        this->clearSurface();
    }
}

void NPC::resetConversation() {
    this->goTo(this->startTopic, this->startPart);
}

void NPC::tick() {
    // Do something, Taipu
    // Blit 1 letter per call.  For now.  Can be changed later to a delta-time system.
    if (!this->stopAnimating) {
        if (this->paused) {
            // Draw a letter
            // How the doomsday do we transfer this particular bit of Python to SFML?
            this.letterSurface.setString(this->currentLetter);
            sf::FloatRect r = this->letterSurface.getLocalBounds();
            this->letterSize = sf::Vector2<int>(r.width, r.height);
            this->letterSurface.setPosition(this->letterX, this->letterY);
            this->textSurface.draw(this->letterSurface);
            this->letterX += this->letterSize.x;
            this->currentLetterIndex += 1;
        }
    }
}
