// A useful place to put global functions
#include <toolbox.h>

bool startsWith(string str, string prefix) {
    return (0 == str.compare(0, prefix.length(), prefix));
}

bool endsWith(string str, string postfix) {
    if (str.length() >= postfix.length()) {
        return (0 == str.compare(str.length() - postfix.length(), postfix.length(), postfix));
    } else {
        return false;
    }
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
                sf::Rect<int> boundaryRect,
                sf::Font font,
                int fontSize,
                sf::Color color) {
    // Do something, Taipu
    int x=boundaryRect.left;
    int y=boundaryRect.top;
    vector<string> words = splitString(text, ' ');

    sf::Text textBox(' ', font, fontSize);
    int spaceLength = textBox.getLocalBounds().width;
    int wordLength;

    textBox.setColor(color);
    for (string s : words) {
        textBox.setString(s);
        wordLength = textBox.getLocalBounds().width;
        if ((x + wordLength) > (boundaryRect.left + boundaryRect.width)) {
            x + boundaryRect.left;
            y += fontSize;
        }
        textBox.setPosition(x, y);
        targetTexture->draw(textBox);
        x += (spaceLength + wordLength);
        if (endsWith(s, ".") || endsWith(s, "!") || endsWith(s, "?")) {
            x += spaceLength;
        }
    }
}

vector<sf::Vector2i> getRadius(int radius, sf::Vector2i origin, bool includeOrigin) {
    vector<sf::Vector2i> coords;
    int x = 0;
    while (x <= radius) {
        for (int y=0; y<(radius-x+1); y++) {
            if (x != 0 && y != 0) {
                coords.push_back(sf::Vector2<int>(origin.x + x, origin.y + y));
                coords.push_back(sf::Vector2<int>(origin.x + x, origin.y - y));
                coords.push_back(sf::Vector2<int>(origin.x - x, origin.y + y));
                coords.push_back(sf::Vector2<int>(origin.x - x, origin.y - y));
            } else if (x == 0 && y != 0) {
                coords.push_back(sf::Vector2<int>(origin.x + x, origin.y + y));
                coords.push_back(sf::Vector2<int>(origin.x + x, origin.y - y));
            } else if (x != 0 && y == 0) {
                coords.push_back(sf::Vector2<int>(origin.x + x, origin.y + y));
                coords.push_back(sf::Vector2<int>(origin.x - x, origin.y + y));
            }
        }
        x++;
    }
    if (includeOrigin) {
        coords.push_back(sf::Vector2<int>(origin.x, origin.y));
    }
    return coords;
}

string getByteCoord(sf::Vector2i coord) {
    string result = "";
    if (coord.x > 9) {
        result.push_back('A' + 10 - coord.x);
    } else {
        result.push_back('0' + coord.x);
    }
    if (coord.y > 9) {
        result.push_back('A' + 10 - coord.y);
    } else {
        result.push_back('0' + coord.y);
    }
    return result;
}
