#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "GameState.h"

namespace ArkanoidGame
{
    enum class GameStateChangeType
    {
        None,
        Switch
    };

    class Game
    {
    public:
        Game();
        ~Game();

        void UpdateGame(float timeDelta, sf::RenderWindow& window);

        void StartGame();
        void ExitGame();
        void GoToGameOver(int finalScore);

    private:
        void HandleWindowEvents(sf::RenderWindow& window);
        bool Update(float timeDelta);
        void Draw(sf::RenderWindow& window);
        void Shutdown();

        void SwitchStateTo(GameStateType newState, int score = 0);

    private:
        std::vector<std::unique_ptr<GameState>> stateStack;
        GameStateChangeType stateChangeType = GameStateChangeType::None;
        GameStateType pendingGameStateType = GameStateType::None;
        int pendingScore = 0;
    };
}