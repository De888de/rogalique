#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include "Player.h"
#include "Apple.h"
#include "Rock.h"
#include "GameTexts.h"
#include "Background.h"
#include "Leaderboard.h"
#include <SFML/Audio.hpp>

namespace ApplesGame
{
    // Bit flags for game modes
    enum GameModeFlags
    {
        GAME_MODE_NONE = 0,
        GAME_MODE_INFINITE_APPLES = 1 << 0,
        GAME_MODE_WITH_ACCELERATION = 1 << 1,
        GAME_MODE_HIGH_SPEED = 1 << 2,
        GAME_MODE_50_APPLES = 1 << 3,
    };

    // Game States
    enum class GameStateType
    {
        SELECTING_MODE,
        PLAYING,
        GAME_OVER,
        CONFIRM_EXIT,
        PAUSE_MENU,        
        VIEW_LEADERBOARD   
    };

    // The structure for the position in the grid
    struct GridPosition
    {
        int x = 0;
        int y = 0;

        bool operator==(const GridPosition& other) const
        {
            return x == other.x && y == other.y;
        }
    };

    // Hash function for GridPosition (for use in unordered_set/map)
    struct GridPositionHash
    {
        std::size_t operator()(const GridPosition& pos) const
        {
            return std::hash<int>()(pos.x) ^ (std::hash<int>()(pos.y) << 1);
        }
    };

    struct Game
    {
        Player player;
        std::vector<Apple> apples;
        std::vector<Rock> rocks;
        GameTexts texts;
        Background background;

        // New Apple Mesh Containers
        std::unordered_set<GridPosition, GridPositionHash> applePositions;  
        std::unordered_map<GridPosition, int, GridPositionHash> appleGrid;  

        int score = 0;
        GameStateType gameState = GameStateType::SELECTING_MODE;
        GameStateType previousState = GameStateType::SELECTING_MODE;

        // Game mode (bitmask)
        unsigned int gameMode = GAME_MODE_WITH_ACCELERATION;

        
        sf::Text modeSelectionText;
        sf::Text gameOverMenuText;
        sf::Text leaderboardText;
        sf::Text congratulationText;
        sf::Text exitConfirmText;
        sf::Text pauseMenuText;        
        sf::Text leaderboardViewText;   

        // Table of leaderboard
        Leaderboard leaderboard;

        //Resources
        sf::Texture playerTexture;
        sf::Texture appleTexture;
        sf::Texture rockTexture;

        sf::SoundBuffer eatenApplesBuffer;
        sf::SoundBuffer brokenApplesBuffer;
        sf::Sound sound;

        sf::Music music;
    };

    // The main functions of the game
    void InitGame(Game& game);
    void ResetGame(Game& game);
    void ReturnToMenu(Game& game);
    void UpdateGame(Game& game, float deltaTime, sf::RenderWindow& window);
    void DrawGame(Game& game, sf::RenderWindow& window);
    void DeinitializeGame(Game& game);

    // Functions for working with modes
    void SetGameMode(Game& game, unsigned int mode);
    bool HasGameMode(Game& game, unsigned int mode);
    void ApplyGameMode(Game& game);
    void HandleModeSelection(Game& game);

    // New functions for working with the grid
    GridPosition WorldToGrid(const Position2D& worldPos);
    Position2D GridToWorld(const GridPosition& gridPos);
    void UpdateAppleGrid(Game& game);
    bool IsAppleAtPosition(const Game& game, const GridPosition& gridPos);

    // Auxiliary functions for updateGame
    void HandlePlayerInput(Game& game);
    void HandleAppleCollisions(Game& game);
    void HandleRockCollisions(Game& game);
    void HandleBorderCollisions(Game& game);
    void HandleGameOverInput(Game& game);
    void HandleExitConfirmation(Game& game, sf::RenderWindow& window);
    void HandlePauseMenu(Game& game, sf::RenderWindow& window);     
    void HandleLeaderboardView(Game& game);                          

    // Functions for initializing texts
    void InitModeSelectionText(Game& game);
    void InitGameOverMenuText(Game& game);
    void InitLeaderboardText(Game& game);
    void InitCongratulationText(Game& game);
    void InitExitConfirmText(Game& game);
    void InitPauseMenuText(Game& game);        
    void InitLeaderboardViewText(Game& game);   

    // The function of updating the greeting text
    void UpdateCongratulationText(Game& game);
}