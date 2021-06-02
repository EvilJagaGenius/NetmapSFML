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
void InputBox::render(sf::RenderWindow* window, Netmap_Playable* playable) {
    cout << "Called InputBox::render()\n";
}
void InputBox::takeInput(sf::Event event, Netmap_Playable* playable) {
    cout << "Called InputBox::takeInput()\n";
}
void InputBox::setFocus(string focus) {}
void InputBox::setFocus(DataBattlePiece* focus) {}
void InputBox::setSubFocus(int subFocus) {}
string InputBox::getFocus() {
    return "";
}
int InputBox::getSubFocus() {
    return -1;
}
