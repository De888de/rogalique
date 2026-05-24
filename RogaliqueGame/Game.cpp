#include "Game.h"
#include <iostream>

namespace RoqaliqueGame
{
    
    Game* g_Game = nullptr;
    

    Game::Game()
    {
        std::cout << "[Game] Constructor" << std::endl;
    }

    Game::~Game()
    {
        std::cout << "[Game] Destructor" << std::endl;
    }

    void Game::Run()
    {
        std::cout << "[Game] Running..." << std::endl;
    }
}
