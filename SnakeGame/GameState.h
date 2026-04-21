#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

namespace ArkanoidGame
{
    class Game;
    class Block;
    class Platform;
    class Ball;
    Game& GetGame();

    enum class GameStateType
    {
        None,
        Playing,
        GameOver,
        Win
    };

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

    // ====================== GameStatePlayingData ======================
    class GameStatePlayingData
    {
    private:
        std::unique_ptr<Platform> platform_;
        std::unique_ptr<Ball> ball_;
        std::vector<std::unique_ptr<Block>> blocks_;

        int score = 0;
        int lives = 3;

        sf::Font font;
        sf::Text scoreText;
        sf::Text livesText;
        sf::Text controlsText;

        void InitText();
        void InitBlocks();
        bool IsWinCondition() const;

    public:
        GameStatePlayingData();

        void Update(float timeDelta);
        void Draw(sf::RenderWindow& window);

        int GetScore() const { return score; }
        int GetLives() const { return lives; }
        bool IsGameOver() const { return lives <= 0; }
    };

    // ====================== GameStatePlaying ======================
    class GameStatePlaying : public GameState
    {
    private:
        GameStatePlayingData data_;

    public:
        GameStatePlaying();

        void Update(float timeDelta) override;
        void Draw(sf::RenderWindow& window) override;
        void HandleWindowEvent(const sf::Event& event) override;
    };

    // ====================== GameStateGameOver ======================
    class GameStateGameOver : public GameState
    {
    private:
        int finalScore = 0;
        sf::Font font;
        sf::Text gameOverText;
        sf::Text finalScoreText;
        sf::Text pressText;

        void InitText();

    public:
        GameStateGameOver(int score);

        void Update(float timeDelta) override;
        void Draw(sf::RenderWindow& window) override;
        void HandleWindowEvent(const sf::Event& event) override;
    };

    // ====================== GameStateWin ======================
    class GameStateWin : public GameState
    {
    private:
        int finalScore = 0;
        sf::Font font;
        sf::Text winText;
        sf::Text finalScoreText;
        sf::Text pressText;

        void InitText();

    public:
        GameStateWin(int score);

        void Update(float timeDelta) override;
        void Draw(sf::RenderWindow& window) override;
        void HandleWindowEvent(const sf::Event& event) override;
    };
}