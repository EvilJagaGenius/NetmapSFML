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
vector<string> splitString(string str, char delimiter);
sf::Texture imgLoad(string filename);
sf::Font fontLoad(string filename);
void renderText(sf::RenderTexture* targetTexture,
                       string text,
                       //sf::Rect<int> boundaryRect,
                       sf::Font font,
                       sf::Color color);


#endif // TOOLBOX_H_INCLUDED
