#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "LevelLoader.h"
#include "BlockCounter.h"
#include "BonusSpawner.h"
#include "BonusStrategies.h"

namespace RoqaliqueGame
{
    class Platform;
    class Ball;
    class Block;

    class GameStatePlayingData
    {
    private:
        std::unique_ptr<Platform> platform_;
        std::unique_ptr<Ball> ball_;
        std::vector<std::unique_ptr<Block>> blocks_;

        int score = 0;
        int lives = 3;
        int currentLevel = 0;

        sf::Font font;
        sf::Text scoreText;
        sf::Text livesText;
        sf::Text controlsText;
        sf::Text levelNameText;

        LevelLoader levelLoader;
        std::shared_ptr<BlockCounter> blockCounter;

        BonusSpawner bonusSpawner;
        std::unique_ptr<FireBallStrategy> fireBallStrategy;
        std::unique_ptr<BigPlatformStrategy> bigPlatformStrategy;
        bool isDestroyAllMode = false;
        float destroyAllTimer = 0.0f;

        void InitText();
        std::unique_ptr<Block> CreateBlockByType(BlockType type, const sf::Vector2f& position);
        void LoadLevel(int levelNumber);
        void NextLevel();
        void CreateTestLevel();
        void SetupBlockCounter();
        void OnAllBlocksDestroyed();
        void ApplyBonus(BonusType type);
        void UpdateBonuses(float timeDelta);

    public:
        GameStatePlayingData(int startLevel = 0, int startScore = 0, int startLives = 3);
        void SetGameParams(int level, int score, int lives);
        void RestartLevel();

        void Update(float timeDelta);
        void Draw(sf::RenderWindow& window);

        int GetScore() const { return score; }
        int GetLives() const { return lives; }
        int GetCurrentLevel() const { return currentLevel; }
        bool IsGameOver() const { return lives <= 0; }

        void SetLives(int newLives) { lives = newLives; livesText.setString("Lives: " + std::to_string(lives)); }
        void SetScore(int newScore) { score = newScore; scoreText.setString("Score: " + std::to_string(score)); }
    };
}

