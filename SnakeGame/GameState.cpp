#include "GameState.h"
#include "Platform.h"
#include "Ball.h"
#include "GameSettings.h"
#include "Game.h"
#include <string>
#include <iostream>

namespace ArkanoidGame
{
    Game& GetGame();
}

namespace ArkanoidGame
{
    //  GameState  
    GameState::GameState(GameStateType type, bool isExclusivelyVisible)
        : type_(type), isExclusivelyVisible_(isExclusivelyVisible)
    {
    }

    //  GameStatePlayingData 
    void GameStatePlayingData::InitText()
    {
        if (!font.loadFromFile("Resources/Fonts/Roboto-Black.ttf"))
        {
            font.loadFromFile("D:/xyz/HW_03/xyz-cpp-course/ArkanoidGame/Resources/Fonts/Roboto-Black.ttf");
        }

        scoreText.setFont(font);
        scoreText.setCharacterSize(20);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setStyle(sf::Text::Bold);
        scoreText.setPosition(SETTINGS.SCREEN_WIDTH - 120.f, 15.f);
        scoreText.setString("Score: 0");

        livesText.setFont(font);
        livesText.setCharacterSize(16);
        livesText.setFillColor(sf::Color::Red);
        livesText.setStyle(sf::Text::Bold);
        livesText.setPosition(20.f, 15.f);
        livesText.setString("Lives: 3");

        controlsText.setFont(font);
        controlsText.setCharacterSize(16);
        controlsText.setFillColor(sf::Color(200, 200, 100));
        controlsText.setPosition(20.f, 50.f);
        controlsText.setString("< A / D >  move    ESC exit");
    }

    GameStatePlayingData::GameStatePlayingData()
    {
        platform_ = std::make_unique<Platform>(
            sf::Vector2f(SETTINGS.SCREEN_WIDTH / 2.0f, SETTINGS.SCREEN_HEIGHT - 60.0f));

        ball_ = std::make_unique<Ball>(
            sf::Vector2f(SETTINGS.SCREEN_WIDTH / 2.0f, SETTINGS.SCREEN_HEIGHT / 2.0f));

        InitText();
    }

    void GameStatePlayingData::Update(float timeDelta)
    {
        platform_->Update(timeDelta);
        ball_->Update(timeDelta);

        if (platform_->CheckCollision(*ball_))
        {
            score++;
            scoreText.setString("Score: " + std::to_string(score));
        }

        if (ball_->HasFallen())
        {
            lives--;
            livesText.setString("Lives: " + std::to_string(lives));
            ball_->ClearFallenFlag();

            if (lives <= 0)
            {
                ArkanoidGame::GetGame().GoToGameOver(score);
            }
        }
    }

    void GameStatePlayingData::Draw(sf::RenderWindow& window)
    {
        platform_->Draw(window);
        ball_->Draw(window);
        window.draw(scoreText);
        window.draw(livesText);
        window.draw(controlsText);
    }

    // GameStatePlaying 
    GameStatePlaying::GameStatePlaying()
        : GameState(GameStateType::Playing, true)
    {
    }

    void GameStatePlaying::Update(float timeDelta)
    {
        data_.Update(timeDelta);
    }

    void GameStatePlaying::Draw(sf::RenderWindow& window)
    {
        data_.Draw(window);
    }

    void GameStatePlaying::HandleWindowEvent(const sf::Event& event) {}

    //  GameStateGameOver
    void GameStateGameOver::InitText()
    {
        if (!font.loadFromFile("Resources/Fonts/Roboto-Black.ttf"))
        {
            font.loadFromFile("D:/xyz/HW_03/xyz-cpp-course/ArkanoidGame/Resources/Fonts/Roboto-Black.ttf");
        }

        gameOverText.setFont(font);
        gameOverText.setCharacterSize(60);
        gameOverText.setFillColor(sf::Color::Red);
        gameOverText.setStyle(sf::Text::Bold);
        gameOverText.setString("GAME OVER");
        gameOverText.setPosition(SETTINGS.SCREEN_WIDTH / 2.f - 190.f, 160.f);

        finalScoreText.setFont(font);
        finalScoreText.setCharacterSize(36);
        finalScoreText.setFillColor(sf::Color::White);
        finalScoreText.setString("Final Score: 0");
        finalScoreText.setPosition(SETTINGS.SCREEN_WIDTH / 2.f - 150.f, 260.f);

        pressText.setFont(font);
        pressText.setCharacterSize(24);
        pressText.setFillColor(sf::Color::Yellow);
        pressText.setString("Press ESC to Exit");
        pressText.setPosition(SETTINGS.SCREEN_WIDTH / 2.f - 130.f, 340.f);
    }

    GameStateGameOver::GameStateGameOver(int score)
        : GameState(GameStateType::GameOver, true), finalScore(score)
    {
        InitText();
        finalScoreText.setString("Final Score: " + std::to_string(score));
    }

    void GameStateGameOver::Update(float timeDelta) {}

    void GameStateGameOver::Draw(sf::RenderWindow& window)
    {
        window.draw(gameOverText);
        window.draw(finalScoreText);
        window.draw(pressText);
    }

    void GameStateGameOver::HandleWindowEvent(const sf::Event& event)
    {
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Escape)
            {
                ArkanoidGame::GetGame().ExitGame();
            }
        }
    }
}