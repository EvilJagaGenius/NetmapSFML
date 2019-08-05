#ifndef DATABATTLEEDITOR_H
#define DATABATTLEEDITOR_H

#include "databattle.h"
#include "textinputbox.h"
#include "toolbox.h"

class DataBattleEditor: public Netmap_Playable
{
    public:
        DataBattle* db;
        sf::Vector2i mousePos;
        sf::Sprite gridSprite;
        sf::Sprite programSprite;
        InputBox* currentInputBox;
        char inputBoxType;

        // General-purpose input button
        sf::Text textBox;
        sf::Rect<int> button;
        sf::RectangleShape buttonGraphic;

        DataBattleEditor(string dummy);
        virtual ~DataBattleEditor();
        void newDB(string filename);
        void loadDB(string filename);
        void saveDB(string filename);
        void render(sf::RenderWindow* window);
        string play(sf::RenderWindow* window);

    protected:

    private:
};

#endif // DATABATTLEEDITOR_H
