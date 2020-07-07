#ifndef NETWORKDATABATTLE_H
#define NETWORKDATABATTLE_H

#include <SFML/Network.hpp>

#include "databattle.h"

class NetworkDataBattle : public DataBattle
{
    public:
        NetworkDataBattle();
        virtual ~NetworkDataBattle();

    protected:

    private:
};

#endif // NETWORKDATABATTLE_H
