#include "Game.h"

namespace RoqaliqueGame
{
    Game* g_Game = nullptr;

    Game& GetGame()
    {
        return *g_Game;
    }
}