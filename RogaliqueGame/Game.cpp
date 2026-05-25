#include "Game.h"
#include "GameSettings.h"
#include <iostream>

namespace rogalique
{
    Game* g_Game = nullptr;

    Game::Game()
        : m_hasSave(false)
        , m_isPaused(false)
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
        m_player = std::make_unique<Player>();
        m_player->SetPosition(sf::Vector2f(SETTINGS.SCREEN_WIDTH / 2.0f, SETTINGS.SCREEN_HEIGHT / 2.0f));
        m_hasSave = false;
        m_isPaused = false;
    }

    void Game::ContinueGame()
    {
        std::cout << "[Game] Continue Game" << std::endl;
        // TODO: загрузить сохранение
        StartNewGame(); // временно
    }

    void Game::Update(float deltaTime)
    {
        if (m_isPaused || !m_player) return;
        m_player->Update(deltaTime);
    }

    void Game::Draw(sf::RenderWindow& window)
    {
        if (m_player)
            m_player->Draw(window);
    }

    void Game::Pause()
    {
        m_isPaused = true;
        std::cout << "[Game] Paused" << std::endl;
    }

    void Game::Resume()
    {
        m_isPaused = false;
        std::cout << "[Game] Resumed" << std::endl;
    }

    bool Game::HasSavedProgress() const
    {
        return m_hasSave;
    }
}
