#ifndef DATABATTLEPLAYER_H
#define DATABATTLEPLAYER_H

#include "netmap_playable.h"

class DataBattlePlayer : Netmap_Playable
{
    public:
        DataBattlePlayer();
        virtual ~DataBattlePlayer();
        void render();
        void play(sf::RenderWindow* window);

    protected:

    private:
};

#endif // DATABATTLEPLAYER_H
