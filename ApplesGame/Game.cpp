#include "Game.h"
#include "Math.h"
#include "Constants.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Window.hpp>
#include <cassert>
#include <iostream>
#include <sstream>
#include <iomanip>

namespace ApplesGame
{
    
    GridPosition WorldToGrid(const Position2D& worldPos)
    {
        GridPosition gridPos;
        gridPos.x = static_cast<int>(worldPos.x / APPLE_SIZE);
        gridPos.y = static_cast<int>(worldPos.y / APPLE_SIZE);
        return gridPos;
    }

    
    Position2D GridToWorld(const GridPosition& gridPos)
    {
        Position2D worldPos;
        worldPos.x = gridPos.x * APPLE_SIZE + APPLE_SIZE / 2.f;
        worldPos.y = gridPos.y * APPLE_SIZE + APPLE_SIZE / 2.f;
        return worldPos;
    }

    
    void UpdateAppleGrid(Game& game)
    {
        game.applePositions.clear();
        game.appleGrid.clear();

        for (size_t i = 0; i < game.apples.size(); ++i)
        {
            GridPosition gridPos = WorldToGrid(game.apples[i].position);
            game.applePositions.insert(gridPos);
            game.appleGrid[gridPos] = static_cast<int>(i);
        }
    }

    // Checking for an apple in a position
    bool IsAppleAtPosition(const Game& game, const GridPosition& gridPos)
    {
        return game.applePositions.find(gridPos) != game.applePositions.end();
    }

    // Initializing the pause menu text
    void InitPauseMenuText(Game& game)
    {
        game.pauseMenuText.setFont(game.texts.font);
        game.pauseMenuText.setCharacterSize(20);
        game.pauseMenuText.setFillColor(sf::Color::White);
        game.pauseMenuText.setString(
            "=== PAUSE MENU ===\n\n"
            "BACKSPACE - Continue\n"
            "L - View Leaderboard\n"
            "M - Return to Menu\n"
            "ESC - Exit Game"
        );

        sf::FloatRect textRect = game.pauseMenuText.getLocalBounds();
        game.pauseMenuText.setOrigin(textRect.left + textRect.width / 2.0f,
            textRect.top + textRect.height / 2.0f);
        game.pauseMenuText.setPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f);
    }

    // Initializing the text for viewing the high score table
    void InitLeaderboardViewText(Game& game)
    {
        game.leaderboardViewText.setFont(game.texts.font);
        game.leaderboardViewText.setCharacterSize(24);
        game.leaderboardViewText.setFillColor(sf::Color::Cyan);
        
    }

    // Initialization of menu texts
    void InitModeSelectionText(Game& game)
    {
        game.modeSelectionText.setFont(game.texts.font);
        game.modeSelectionText.setCharacterSize(24);
        game.modeSelectionText.setFillColor(sf::Color::White);
        game.modeSelectionText.setString(
            "SELECT GAME MODE:\n\n"
            "1 - 20 apples, with acceleration\n"
            "2 - 20 apples, without acceleration\n"
            "3 - 50 apples, with acceleration\n"
            "4 - 20 apples, infinite apples, with acceleration\n"
            "5 - 20 apples, infinite apples, without acceleration\n"
            "6 - 50 apples, infinite apples, with acceleration\n"
            "7 - 50 apples, infinite apples, high speed, with acceleration\n\n"
            "L - View Leaderboard\n"
            "ESC - Exit"
        );

        sf::FloatRect textRect = game.modeSelectionText.getLocalBounds();
        game.modeSelectionText.setOrigin(textRect.left + textRect.width / 2.0f,
            textRect.top + textRect.height / 2.0f);
        game.modeSelectionText.setPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f - 50);
    }

    void InitGameOverMenuText(Game& game)
    {
        game.gameOverMenuText.setFont(game.texts.font);
        game.gameOverMenuText.setCharacterSize(20);
        game.gameOverMenuText.setFillColor(sf::Color::Yellow);
        game.gameOverMenuText.setString(
            "Congratulations, you've scored " + std::to_string(game.score) + " points!\n\n"
            "Press SPACE to play again\n"
            "Press L to view Leaderboard\n"
            "Press M to return to menu\n"
            "Press ESC to exit"
        );

        sf::FloatRect textRect = game.gameOverMenuText.getLocalBounds();
        game.gameOverMenuText.setOrigin(textRect.left + textRect.width / 2.0f,
            textRect.top + textRect.height / 2.0f);
        game.gameOverMenuText.setPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f);
    }

    void UpdateGameOverMenuText(Game& game)
    {
        game.gameOverMenuText.setString(
            "Congratulations, you've scored " + std::to_string(game.score) + " points!\n\n"
            "Press SPACE to play again\n"
            "Press L to view Leaderboard\n"
            "Press M to return to menu\n"
            "Press ESC to exit"
        );

        sf::FloatRect textRect = game.gameOverMenuText.getLocalBounds();
        game.gameOverMenuText.setOrigin(textRect.left + textRect.width / 2.0f,
            textRect.top + textRect.height / 2.0f);
        game.gameOverMenuText.setPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f);
    }

    void InitExitConfirmText(Game& game)
    {
        game.exitConfirmText.setFont(game.texts.font);
        game.exitConfirmText.setCharacterSize(20);
        game.exitConfirmText.setFillColor(sf::Color::White);
        game.exitConfirmText.setStyle(sf::Text::Bold);
        game.exitConfirmText.setString(
            "Are you sure you want to exit?\n\n"
            "Y - Yes, exit the game\n"
            "N - No, return"
        );

        sf::FloatRect textRect = game.exitConfirmText.getLocalBounds();
        game.exitConfirmText.setOrigin(textRect.left + textRect.width / 2.0f,
            textRect.top + textRect.height / 2.0f);
        game.exitConfirmText.setPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f);
    }

    void InitLeaderboardText(Game& game)
    {
        game.leaderboardText.setFont(game.texts.font);
        game.leaderboardText.setCharacterSize(20);
        game.leaderboardText.setFillColor(sf::Color::Cyan);
    }

    void InitCongratulationText(Game& game)
    {
        game.congratulationText.setFont(game.texts.font);
        game.congratulationText.setCharacterSize(28);
        game.congratulationText.setFillColor(sf::Color::Green);
        game.congratulationText.setStyle(sf::Text::Bold);
    }

    // Functions for working with modes
    void SetGameMode(Game& game, unsigned int mode)
    {
        game.gameMode = mode;
    }

    bool HasGameMode(Game& game, unsigned int mode)
    {
        return (game.gameMode & mode) != 0;
    }

    void ApplyGameMode(Game& game)
    {
        // We determine the number of apples
        int numApples = HasGameMode(game, GAME_MODE_50_APPLES) ? 50 : 20;

        
        game.apples.clear();
        game.apples.reserve(numApples);

        for (int i = 0; i < numApples; ++i)
        {
            Apple apple;
            apple.Init(game);
            game.apples.push_back(apple);
        }

        // Updating the apple grid
        UpdateAppleGrid(game);

        // Applying speed
        if (HasGameMode(game, GAME_MODE_HIGH_SPEED))
        {
            game.player.speed = INITIAL_SPEED * 1.5f;
        }
        else
        {
            game.player.speed = INITIAL_SPEED;
        }

        std::cout << "Applied game mode with " << game.apples.size() << " apples" << std::endl;
    }

    void HandleModeSelection(Game& game)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
        {
            game.gameMode = GAME_MODE_WITH_ACCELERATION;
            ApplyGameMode(game);
            game.leaderboard.Generate();
            game.gameState = GameStateType::PLAYING;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
        {
            game.gameMode = GAME_MODE_NONE;
            ApplyGameMode(game);
            game.leaderboard.Generate();
            game.gameState = GameStateType::PLAYING;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
        {
            game.gameMode = GAME_MODE_WITH_ACCELERATION | GAME_MODE_50_APPLES;
            ApplyGameMode(game);
            game.leaderboard.Generate();
            game.gameState = GameStateType::PLAYING;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
        {
            game.gameMode = GAME_MODE_WITH_ACCELERATION | GAME_MODE_INFINITE_APPLES;
            ApplyGameMode(game);
            game.leaderboard.Generate();
            game.gameState = GameStateType::PLAYING;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5))
        {
            game.gameMode = GAME_MODE_INFINITE_APPLES;
            ApplyGameMode(game);
            game.leaderboard.Generate();
            game.gameState = GameStateType::PLAYING;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6))
        {
            game.gameMode = GAME_MODE_WITH_ACCELERATION | GAME_MODE_INFINITE_APPLES | GAME_MODE_50_APPLES;
            ApplyGameMode(game);
            game.leaderboard.Generate();
            game.gameState = GameStateType::PLAYING;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num7))
        {
            game.gameMode = GAME_MODE_WITH_ACCELERATION | GAME_MODE_INFINITE_APPLES | GAME_MODE_50_APPLES | GAME_MODE_HIGH_SPEED;
            ApplyGameMode(game);
            game.leaderboard.Generate();
            game.gameState = GameStateType::PLAYING;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
        {
            
            game.leaderboard.Generate();
            game.previousState = game.gameState;
            game.gameState = GameStateType::VIEW_LEADERBOARD;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            game.previousState = game.gameState;
            game.gameState = GameStateType::CONFIRM_EXIT;
        }
    }

    // A function for processing the pause menu
    void HandlePauseMenu(Game& game, sf::RenderWindow& window)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace))
        {
            game.gameState = GameStateType::PLAYING; // Continue playing on the Backspace
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
        {
            game.leaderboard.Generate(); 
            game.previousState = game.gameState;
            game.gameState = GameStateType::VIEW_LEADERBOARD; // table review
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
        {
            ReturnToMenu(game); // Exit the menu
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            game.previousState = game.gameState;
            game.gameState = GameStateType::CONFIRM_EXIT; // Exit the game
        }
    }

    // A function for processing leaderboard
    void HandleLeaderboardView(Game& game)
    {
        // Updating the table text
        game.leaderboardViewText.setString("=== LEADERBOARD ===\n\n" + game.leaderboard.Format() +
            "\n\nPress BACKSPACE to return");

        // Centering the text
        sf::FloatRect textRect = game.leaderboardViewText.getLocalBounds();
        game.leaderboardViewText.setOrigin(textRect.left + textRect.width / 2.0f,
            textRect.top + textRect.height / 2.0f);
        game.leaderboardViewText.setPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f);

        // We return by pressing Backspace
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace))
        {
            game.gameState = game.previousState;
        }
    }

    // Basic initialization of the game
    void InitGame(Game& game)
    {
        // Loading resources
        assert(game.playerTexture.loadFromFile(RESOURCES_PATH + "Player.png"));
        assert(game.appleTexture.loadFromFile(RESOURCES_PATH + "Apple.png"));
        assert(game.rockTexture.loadFromFile(RESOURCES_PATH + "Rock.png"));
        assert(game.eatenApplesBuffer.loadFromFile(RESOURCES_PATH + "apple_eat.wav"));
        assert(game.brokenApplesBuffer.loadFromFile(RESOURCES_PATH + "game_over.wav"));

        // Music
        game.music.openFromFile(RESOURCES_PATH + "background_music.ogg");
        game.music.setLoop(true);
        game.music.play();

        // Initializing components
        game.background.Init();
        game.texts.Init();
        game.player.Init(game);

        // Initialization of menu texts
        InitModeSelectionText(game);
        InitGameOverMenuText(game);
        InitLeaderboardText(game);
        InitCongratulationText(game);
        InitExitConfirmText(game);
        InitPauseMenuText(game);
        InitLeaderboardViewText(game);

        // Initialization of stones (vector)
        game.rocks.clear();
        game.rocks.reserve(NUM_ROCKS);

        for (int i = 0; i < NUM_ROCKS; ++i)
        {
            Rock rock;
            rock.Init(game);
            game.rocks.push_back(rock);
        }

        // Initial state
        game.score = 0;
        game.gameState = GameStateType::SELECTING_MODE;
        game.apples.clear();
        game.applePositions.clear();
        game.appleGrid.clear();
        game.leaderboard.Clear();
    }

    void ResetGame(Game& game)
    {
        game.background.SetNormalColor();
        game.player.Reset();

       
        ApplyGameMode(game);

        // Updating leaderboard
        game.leaderboard.UpdatePlayerScore(game.score);

        // Dropping the stones
        for (auto& rock : game.rocks)
        {
            rock.Reset();
        }

        game.score = 0;
        game.texts.UpdateScore(0);
        game.gameState = GameStateType::PLAYING;
    }

    void ReturnToMenu(Game& game)
    {
        game.apples.clear();
        game.applePositions.clear();
        game.appleGrid.clear();
        game.background.SetNormalColor();
        game.player.Reset();

        for (auto& rock : game.rocks)
        {
            rock.Reset();
        }

        game.score = 0;
        game.texts.UpdateScore(0);
        game.gameState = GameStateType::SELECTING_MODE;
    }

    // Game Features
    void HandlePlayerInput(Game& game)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            game.player.direction = PlayerDirection::Right;
            game.player.sprite.setRotation(0.f);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            game.player.direction = PlayerDirection::Up;
            game.player.sprite.setRotation(-90.f);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            game.player.direction = PlayerDirection::Left;
            game.player.sprite.setRotation(0.f);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            game.player.direction = PlayerDirection::Down;
            game.player.sprite.setRotation(90.f);
        }
    }

    // Updated apple collision handling feature
    void HandleAppleCollisions(Game& game)
    {
        GridPosition playerGridPos = WorldToGrid(game.player.position);

        // Quick check via unordered_set
        if (IsAppleAtPosition(game, playerGridPos))
        {
            // We find the apple index through unordered_map
            int appleIndex = game.appleGrid[playerGridPos];

            // The sound of eating
            game.sound.setBuffer(game.eatenApplesBuffer);
            game.sound.play();

            if (HasGameMode(game, GAME_MODE_INFINITE_APPLES))
            {
                game.apples[appleIndex].Reset();
            }
            else
            {
                game.apples.erase(game.apples.begin() + appleIndex);
            }

            game.score++;

            if (HasGameMode(game, GAME_MODE_WITH_ACCELERATION))
            {
                game.player.speed += ACCELERATION;
            }

            game.texts.UpdateScore(game.score);

           
            UpdateAppleGrid(game);

            if (!HasGameMode(game, GAME_MODE_INFINITE_APPLES) && game.apples.empty())
            {
                game.leaderboard.UpdatePlayerScore(game.score);
                game.gameState = GameStateType::GAME_OVER;
            }
        }
    }

    void HandleRockCollisions(Game& game)
    {
        for (const auto& rock : game.rocks)
        {
            if (IsRectanglesCollide(game.player.position, { PLAYER_SIZE, PLAYER_SIZE },
                rock.position, { ROCK_SIZE, ROCK_SIZE }))
            {
                game.sound.setBuffer(game.brokenApplesBuffer);
                game.sound.play();
                game.leaderboard.UpdatePlayerScore(game.score);
                game.gameState = GameStateType::GAME_OVER;
                break;
            }
        }
    }

    void HandleBorderCollisions(Game& game)
    {
        if (game.player.position.x - PLAYER_SIZE / 2.f < 0.f ||
            game.player.position.x + PLAYER_SIZE / 2.f > SCREEN_WIDTH ||
            game.player.position.y - PLAYER_SIZE / 2.f < 0.f ||
            game.player.position.y + PLAYER_SIZE / 2.f > SCREEN_HEIGHT)
        {
            game.sound.setBuffer(game.brokenApplesBuffer);
            game.sound.play();
            game.leaderboard.UpdatePlayerScore(game.score);
            game.gameState = GameStateType::GAME_OVER;
        }
    }

    void HandleGameOverInput(Game& game)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            ResetGame(game);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
        {
            game.leaderboard.Generate(); 
            game.previousState = game.gameState;
            game.gameState = GameStateType::VIEW_LEADERBOARD;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
        {
            ReturnToMenu(game);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            game.previousState = game.gameState;
            game.gameState = GameStateType::CONFIRM_EXIT;
        }
    }

    void HandleExitConfirmation(Game& game, sf::RenderWindow& window)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
        {
            window.close();
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
        {
            game.gameState = game.previousState;
        }
    }

    // Main update function
    void UpdateGame(Game& game, float deltaTime, sf::RenderWindow& window)
    {
        if (deltaTime > 0.1f) deltaTime = 0.1f;

        switch (game.gameState)
        {
        case GameStateType::SELECTING_MODE:
            HandleModeSelection(game);
            break;

        case GameStateType::PLAYING:
            HandlePlayerInput(game);
            game.player.UpdatePosition(deltaTime);
            HandleAppleCollisions(game);
            HandleRockCollisions(game);
            HandleBorderCollisions(game);

            // Opening the pause menu using the P key
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
            {
                game.previousState = game.gameState;
                game.gameState = GameStateType::PAUSE_MENU;
            }
            break;

        case GameStateType::GAME_OVER:
            UpdateGameOverMenuText(game); // Updating the text with the current account
            HandleGameOverInput(game);
            break;

        case GameStateType::CONFIRM_EXIT:
            HandleExitConfirmation(game, window);
            break;

        case GameStateType::PAUSE_MENU:
            HandlePauseMenu(game, window);
            break;

        case GameStateType::VIEW_LEADERBOARD:
            HandleLeaderboardView(game);
            break;
        }
    }

    // rendering function
    void DrawGame(Game& game, sf::RenderWindow& window)
    {
        window.draw(game.background.sprite);

        sf::FloatRect leaderboardRect;
        sf::FloatRect menuRect;

        switch (game.gameState)
        {
        case GameStateType::SELECTING_MODE:
        {
            window.draw(game.modeSelectionText);
            break;
        }

        case GameStateType::PLAYING:
        {
            window.draw(game.player.sprite);

            for (const auto& apple : game.apples)
            {
                window.draw(apple.sprite);
            }

            for (const auto& rock : game.rocks)
            {
                window.draw(rock.sprite);
            }

            window.draw(game.texts.scoreText);
            break;
        }

        case GameStateType::GAME_OVER:
        {
            game.background.SetGameOverColor();
            window.draw(game.background.sprite);

            window.draw(game.gameOverMenuText);
            break;
        }

        case GameStateType::CONFIRM_EXIT:
        {
            sf::RectangleShape darkOverlay;
            darkOverlay.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
            darkOverlay.setFillColor(sf::Color(0, 0, 0, 200));
            window.draw(darkOverlay);

            window.draw(game.exitConfirmText);
            break;
        }

        case GameStateType::PAUSE_MENU:
        {
            sf::RectangleShape darkOverlay;
            darkOverlay.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
            darkOverlay.setFillColor(sf::Color(0, 0, 0, 200));
            window.draw(darkOverlay);

            window.draw(game.pauseMenuText);
            break;
        }

        case GameStateType::VIEW_LEADERBOARD:
        {
            sf::RectangleShape darkOverlay;
            darkOverlay.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
            darkOverlay.setFillColor(sf::Color(0, 0, 0, 200));
            window.draw(darkOverlay);

            window.draw(game.leaderboardViewText);
            break;
        }
        }
    }

    void DeinitializeGame(Game& game)
    {
        game.apples.clear();
        game.rocks.clear();
        game.applePositions.clear();
        game.appleGrid.clear();
        game.leaderboard.Clear();
    }
}