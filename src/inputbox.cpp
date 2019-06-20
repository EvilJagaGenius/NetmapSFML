#include "inputbox.h"

InputBox::InputBox()
{
    //ctor
}

InputBox::~InputBox()
{
    //dtor
}
void InputBox::setMousePos(sf::Vector2i mousePos) {
}
void InputBox::render(sf::RenderWindow* window) {
}
void InputBox::takeInput(sf::Event event, Netmap_Playable* playable) {
    cout << "Called InputBox version of takeInput\n";
}
void InputBox::takeInput(sf::Event event, DataBattle* playable) {
    cout << "Called InputBox/DataBattle version of takeInput\n";
}

void InputBox::setFocus(string focus) {
}
string InputBox::getFocus() {
    return "";
}
