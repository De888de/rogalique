#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

#include "Snake.h"
#include "Apple.h"
#include "Rock.h"
#include "Grid.h"
#include "Leaderboard.h"
#include "SoundManager.h"
#include "MenuManager.h"
#include "SoundPopup.h"
#include "DifficultyMenu.h"
#include "Constants.h"

namespace SnakeGame {

    enum class GameState {
        MAIN_MENU,
        PLAYING,
        PAUSED,
        MENU,
        GAME_OVER,
        DIFFICULTY_SELECT,
        LEADERBOARD,
        SOUND_POPUP
    };

    class Game {
    private:
        // ==================== Основные игровые объекты ====================
        Snake snake;
        Apple apple;
        Grid grid;
        Leaderboard leaderboard;
        SoundManager soundManager;
        sf::Font font;
        MenuManager menuManager;
        std::unique_ptr<SoundPopup> soundPopup;

        // ==================== Состояние игры ====================
        GameState state = GameState::MAIN_MENU;
        Difficulty m_currentDifficulty = Difficulty::MEDIUM;
        int score = 0;
        float moveTimer = 0.0f;


       

        // ==================== Объекты уровня ====================
        std::vector<Rock> rocks;
        GridPosition portalPos1{};
        GridPosition portalPos2{};

        // ==================== Ресурсы ====================
        sf::Texture appleTexture;
        sf::Text scoreText;
        sf::Text gameOverText;
        sf::Text finalScoreText;
        sf::Text menuHintText;
        sf::CircleShape portalShape;

       

        // ==================== ФЛАГИ ДЛЯ КЛАВИАТУРЫ (централизованные) ====================
        bool upPressed = false;
        bool downPressed = false;
        bool enterPressed = false;
        bool escPressed = false;
        bool sPressed = false;
        bool pPressed = false;
        bool mPressed = false;        // для вызова игрового меню

        void ResetInputFlags();

        // ==================== Вспомогательные методы ====================
        void SetupTexts();
        void LoadTextures();
        void GenerateLevel();
        void ClearLevel();
        bool IsPositionOccupied(const GridPosition& pos);
        const DifficultySettings& GetDifficultySettings(Difficulty diff) const;

        //void HandleStartScreenInput();


        float portalGlowScale = 1.0f;
        float portalGlowTargetScale = 1.0f;
        float portalGlowSpeed = 0.2f;

       

        // Методы порталов
        void GeneratePortals();
        void TeleportSnakeThroughPortal(const GridPosition& from, const GridPosition& to);
        int GetManhattanDistance(const GridPosition& a, const GridPosition& b) const;
        bool IsFarFromObstacles(const GridPosition& pos, int minDist) const;

        void GenerateMagicApple();

        void GeneratePoisonApple();

        // ==================== ВОЛШЕБНОЕ ЯБЛОКО ====================
        sf::Texture magicAppleTexture;
        GridPosition magicApplePos{};
        bool magicAppleActive = false;
        int applesEatenSinceLastMagic = 0;     
        const int MAGIC_APPLE_MIN_INTERVAL = 5;
        const int MAGIC_APPLE_MAX_INTERVAL = 10;

        // ==================== ОТРАВЛЯЮЩЕЕ ЯБЛОКО ====================
          
        GridPosition poisonApplePos{};
        bool poisonAppleActive = false;
        float poisonEffectTimer = 0.0f;
        bool isPoisoned = false;

        int applesEatenSinceLastPoison = 0;

        const float POISON_DURATION = 15.0f;
        const int POISON_APPLE_MIN_INTERVAL = 8;
        const int POISON_APPLE_MAX_INTERVAL = 14;

        // Обработчики ввода
        void HandleMainMenuInput();
        void HandleDifficultyInput();
        void HandleGameMenuInput();
        void HandlePlayingInput();
        void HandleSoundPopupInput();

        void CheckCollisions();
        void DrawGame(sf::RenderWindow& window, float deltaTime = 0.0f);

        
        bool justTeleported = false;   // флаг, что в этом кадре произошла телепортация

    public:
        Game();
        void Init();
        void Reset();
        void HandleInput();
        void Update(float deltaTime);
        void Draw(sf::RenderWindow& window);
        void GameOver();
    };

} 