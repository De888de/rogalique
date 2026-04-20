#include "Game.h"
#include "GameState.h"
#include <iostream>

namespace ArkanoidGame
{
    Game::Game()
    {
        SwitchStateTo(GameStateType::Playing);
    }

    Game::~Game()
    {
        Shutdown();
    }

    void Game::HandleWindowEvents(sf::RenderWindow& window)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (!stateStack.empty())
                stateStack.back()->HandleWindowEvent(event);
        }
    }

    bool Game::Update(float timeDelta)
    {
        if (stateChangeType == GameStateChangeType::Switch)
            stateStack.clear();

        if (pendingGameStateType != GameStateType::None)
        {
            if (pendingGameStateType == GameStateType::Playing)
            {
                stateStack.push_back(std::make_unique<GameStatePlaying>());
            }
            else if (pendingGameStateType == GameStateType::GameOver)
            {
                stateStack.push_back(std::make_unique<GameStateGameOver>(pendingScore));
            }
            else if (pendingGameStateType == GameStateType::Win)
            {
                stateStack.push_back(std::make_unique<GameStateWin>(pendingScore));
            }

            pendingGameStateType = GameStateType::None;
            pendingScore = 0;
        }

        stateChangeType = GameStateChangeType::None;

        if (!stateStack.empty())
        {
            stateStack.back()->Update(timeDelta);
            return true;
        }
        return false;
    }

    void Game::Draw(sf::RenderWindow& window)
    {
        if (!stateStack.empty())
            stateStack.back()->Draw(window);
    }

    void Game::Shutdown()
    {
        stateStack.clear();
    }

    void Game::SwitchStateTo(GameStateType newState, int score)
    {
        pendingGameStateType = newState;
        pendingScore = score;
        stateChangeType = GameStateChangeType::Switch;
    }

    void Game::GoToGameOver(int finalScore)
    {
        SwitchStateTo(GameStateType::GameOver, finalScore);
    }

    void Game::GoToWin(int finalScore)
    {
        SwitchStateTo(GameStateType::Win, finalScore);
    }

    void Game::StartGame()
    {
        stateStack.clear();
        stateChangeType = GameStateChangeType::None;
        pendingGameStateType = GameStateType::None;
        pendingScore = 0;

        SwitchStateTo(GameStateType::Playing);
    }

    void Game::ExitGame()
    {
        SwitchStateTo(GameStateType::None);
    }

    void Game::UpdateGame(float timeDelta, sf::RenderWindow& window)
    {
        HandleWindowEvents(window);

        if (Update(timeDelta))
        {
            window.clear();
            Draw(window);
            window.display();
        }
        else
        {
            window.close();
        }
    }
}