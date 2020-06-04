#ifndef DATABATTLEPLAYER_H
#define DATABATTLEPLAYER_H

#include "netmap_playable.h"
#include "databattle.h"

class DataBattlePlayer : public Netmap_Playable
{
    public:
        DataBattle* db;
        sf::Sprite bkgSprite;
        sf::Sprite gridSprite;
        sf::Sprite programSprite;
        string musicFilename;
        sf::Music musicTrack;
        sf::Vector2i mousePos;
        sf::Vector2i nButton, sButton, eButton, wButton;
        vector<sf::Vector2i> moveArea;
        vector<sf::Vector2i> aimArea;

        DataBattlePlayer();
        DataBattlePlayer(DataBattle* db);
        virtual ~DataBattlePlayer();
        void render(sf::RenderWindow* window);
        string play(sf::RenderWindow* window);
        void setDB(DataBattle* db);

    protected:

    private:
};

#endif // DATABATTLEPLAYER_H
