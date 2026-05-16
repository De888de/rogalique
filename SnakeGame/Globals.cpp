#include "Game.h"

namespace ArkanoidGame
{
    Game* g_Game = nullptr;

    Game& GetGame()
    {
        return *g_Game;
    }
}