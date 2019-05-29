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
