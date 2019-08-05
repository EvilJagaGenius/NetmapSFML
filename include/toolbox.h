// A useful place to put global functions
#ifndef TOOLBOX_H_INCLUDED
#define TOOLBOX_H_INCLUDED

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <SFML/Graphics.hpp>

using namespace std;



bool startsWith(string str, string prefix);
bool endsWith(string str, string postfix);
vector<string> splitString(string str, char delimiter);
sf::Texture imgLoad(string filename);
sf::Font fontLoad(string filename);
void renderText(sf::RenderTexture* targetTexture,
                       string text,
                       sf::Rect<int> boundaryRect,
                       sf::Font font,
                       int fontSize,
                       sf::Color color);
string getByteCoord(sf::Vector2i coord);
vector<sf::Vector2i> getRadius(int radius, sf::Vector2i origin, bool includeOrigin=false);


#endif // TOOLBOX_H_INCLUDED
