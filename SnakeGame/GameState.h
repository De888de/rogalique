#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "GameStatePlayingData.h"
#include "PauseMenu.h"
#include "Leaderboard.h"

namespace ArkanoidGame
{
    class Game;
    Game& GetGame();

    enum class GameStateType { None, Playing, GameOver, Win };

    class GameState
    {
    public:
        explicit GameState(GameStateType type, bool exclusivelyVisible = true);
        virtual ~GameState() = default;
        virtual void Update(float timeDelta) = 0;
        virtual void Draw(sf::RenderWindow& window) = 0;
        virtual void HandleWindowEvent(const sf::Event& event) = 0;
        GameStateType GetType() const { return type_; }
        bool IsExclusivelyVisible() const { return isExclusivelyVisible_; }

    protected:
        GameStateType type_;
        bool isExclusivelyVisible_;
    };

    class GameStatePlaying : public GameState
    {
    private:
        GameStatePlayingData data_;
        PauseMenu pauseMenu;
        bool isPaused = false;

    public:
        GameStatePlaying(int startLevel = 0, int startScore = 0, int startLives = 3);
        void Update(float timeDelta) override;
        void Draw(sf::RenderWindow& window) override;
        void HandleWindowEvent(const sf::Event& event) override;
        void Resume() { isPaused = false; pauseMenu.Close(); }
    };

    class GameStateGameOver : public GameState
    {
    private:
        int finalScore = 0;
        sf::Font font;
        sf::Text gameOverText, finalScoreText, pressText, leaderboardText;
        Leaderboard leaderboard;
        void InitText();
        void UpdateLeaderboardText();

    public:
        GameStateGameOver(int score);
        void Update(float timeDelta) override;
        void Draw(sf::RenderWindow& window) override;
        void HandleWindowEvent(const sf::Event& event) override;
    };

    class GameStateWin : public GameState
    {
    private:
        int finalScore = 0;
        sf::Font font;
        sf::Text winText, finalScoreText, pressText, leaderboardText;
        Leaderboard leaderboard;
        void InitText();
        void UpdateLeaderboardText();

    public:
        GameStateWin(int score);
        void Update(float timeDelta) override;
        void Draw(sf::RenderWindow& window) override;
        void HandleWindowEvent(const sf::Event& event) override;
    };
}