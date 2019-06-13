// A useful place to put global functions
#include <toolbox.h>

bool startsWith(string str, string prefix) {
    return (str.compare(0, prefix.length(), prefix) == 0);
}

vector<string> splitString(string str, char delimiter) {
    vector<string> results;
    istringstream stream(str);
    while (!stream.eof()) {
        string x;
        getline(stream, x, delimiter);
        results.push_back(x);
    }
    return results;
}

sf::Texture imgLoad(string filename) {
    sf::Texture texture;
    texture.loadFromFile(filename);
    return texture;
}

sf::Font fontLoad(string filename) {
    sf::Font newFont;
    newFont.loadFromFile(filename);
    return newFont;
}

void renderText(sf::RenderTexture* targetTexture,
                string text,
                //sf::Rect<int> boundaryRect,
                sf::Font font,
                sf::Color color) {
    // Do something, Taipu
    sf::Text textBox(text, font, 12);
    targetTexture->draw(textBox);
}
