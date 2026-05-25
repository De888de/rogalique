#include "Game.h"
#include <iostream>

namespace rogalique
{
    Game* g_Game = nullptr;

    Game::Game()
    {
        g_Game = this;
        std::cout << "[Game] Constructor" << std::endl;
    }

    Game::~Game()
    {
        g_Game = nullptr;
        std::cout << "[Game] Destructor" << std::endl;
    }

    void Game::StartNewGame()
    {
        std::cout << "[Game] Start New Game" << std::endl;
    }

    void Game::ContinueGame()
    {
        std::cout << "[Game] Continue Game" << std::endl;
    }

    void Game::Update(float deltaTime)
    {
        // Игровая логика будет здесь позже
    }

    void Game::Pause()
    {
        std::cout << "[Game] Paused" << std::endl;
    }

    void Game::Resume()
    {
        std::cout << "[Game] Resumed" << std::endl;
    }

    bool Game::HasSavedProgress() const
    {
        return false;
    }
}
