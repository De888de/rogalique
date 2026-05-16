#include "GameState.h"
#include "Game.h"
#include "GameSettings.h"
#include <iostream>

namespace RoqaliqueGame
{
    Game& GetGame();

    // ====================== GameState ======================
    GameState::GameState(GameStateType type, bool isExclusivelyVisible)
        : type_(type), isExclusivelyVisible_(isExclusivelyVisible) {
    }

    // ====================== GameStatePlaying ======================
    GameStatePlaying::GameStatePlaying(int startLevel, int startScore, int startLives)
        : GameState(GameStateType::Playing, true), data_(startLevel, startScore, startLives)
    {
        pauseMenu.SetOnResume([this]() { isPaused = false; pauseMenu.Close(); });
        pauseMenu.SetOnSaveAndExit([this]() {
            GetGame().SaveCurrentProgress(data_.GetLives(), data_.GetScore(), data_.GetCurrentLevel());
            GetGame().ReturnToMenu();
            });
        pauseMenu.SetOnRestartLevel([this]() {
            data_.RestartLevel();
            isPaused = false;
            pauseMenu.Close();
            });
    }

    void GameStatePlaying::Update(float timeDelta)
    {
        if (!isPaused) data_.Update(timeDelta);
    }

    void GameStatePlaying::Draw(sf::RenderWindow& window)
    {
        data_.Draw(window);
        if (isPaused) pauseMenu.Draw(window);
    }

    void GameStatePlaying::HandleWindowEvent(const sf::Event& event)
    {
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
        {
            isPaused = !isPaused;
            if (isPaused) pauseMenu.Open();
            else pauseMenu.Close();
            return;
        }
        if (isPaused) pauseMenu.HandleInput(event);
    }

    // ====================== GameStateGameOver ======================
    void GameStateGameOver::InitText()
    {
        if (!font.loadFromFile("Resources/Fonts/Roboto-Black.ttf"))
            font.loadFromFile("D:/xyz/HW_03/xyz-cpp-course/ArkanoidGame/Resources/Fonts/Roboto-Black.ttf");

        gameOverText.setFont(font);
        gameOverText.setCharacterSize(55);
        gameOverText.setFillColor(sf::Color::Red);
        gameOverText.setString("GAME OVER");
        sf::FloatRect gameOverBounds = gameOverText.getLocalBounds();
        gameOverText.setPosition(SETTINGS.SCREEN_WIDTH / 2.f - gameOverBounds.width / 2.f, 60.f);

        finalScoreText.setFont(font);
        finalScoreText.setCharacterSize(28);
        finalScoreText.setFillColor(sf::Color::White);
        finalScoreText.setPosition(SETTINGS.SCREEN_WIDTH / 2.f - 130.f, 130.f);

        leaderboardText.setFont(font);
        leaderboardText.setCharacterSize(20);
        leaderboardText.setFillColor(sf::Color(200, 200, 100));
        leaderboardText.setPosition(80.f, 180.f);

        pressText.setFont(font);
        pressText.setCharacterSize(20);
        pressText.setFillColor(sf::Color::Yellow);
        pressText.setString("Press R or SPACE to Restart    ESC to Menu");
        sf::FloatRect pressBounds = pressText.getLocalBounds();
        pressText.setPosition(SETTINGS.SCREEN_WIDTH / 2.f - pressBounds.width / 2.f, SETTINGS.SCREEN_HEIGHT - 80.f);
    }

    void GameStateGameOver::UpdateLeaderboardText()
    {
        leaderboardText.setString(leaderboard.Format());
    }

    GameStateGameOver::GameStateGameOver(int score)
        : GameState(GameStateType::GameOver, true), finalScore(score)
    {
        InitText();
        finalScoreText.setString("YOUR SCORE: " + std::to_string(score));
        leaderboard.UpdatePlayerScore(score);
        UpdateLeaderboardText();
    }

    void GameStateGameOver::Update(float timeDelta) {}

    void GameStateGameOver::Draw(sf::RenderWindow& window)
    {
        window.draw(gameOverText);
        window.draw(finalScoreText);
        window.draw(leaderboardText);
        window.draw(pressText);
    }

    void GameStateGameOver::HandleWindowEvent(const sf::Event& event)
    {
        if (event.type == sf::Event::KeyPressed)
        {
            switch (event.key.code)
            {
            case sf::Keyboard::R:
            case sf::Keyboard::Space:
                GetGame().StartGame();
                break;
            case sf::Keyboard::Escape:
                GetGame().ReturnToMenu();
                break;
            default: break;
            }
        }
    }

    // ====================== GameStateWin ======================
    void GameStateWin::InitText()
    {
        if (!font.loadFromFile("Resources/Fonts/Roboto-Black.ttf"))
            font.loadFromFile("D:/xyz/HW_03/xyz-cpp-course/ArkanoidGame/Resources/Fonts/Roboto-Black.ttf");

        winText.setFont(font);
        winText.setCharacterSize(55);
        winText.setFillColor(sf::Color::Green);
        winText.setString("YOU WIN!");
        sf::FloatRect winBounds = winText.getLocalBounds();
        winText.setPosition(SETTINGS.SCREEN_WIDTH / 2.f - winBounds.width / 2.f, 60.f);

        finalScoreText.setFont(font);
        finalScoreText.setCharacterSize(28);
        finalScoreText.setFillColor(sf::Color::White);
        finalScoreText.setPosition(SETTINGS.SCREEN_WIDTH / 2.f - 130.f, 130.f);

        leaderboardText.setFont(font);
        leaderboardText.setCharacterSize(20);
        leaderboardText.setFillColor(sf::Color(200, 200, 100));
        leaderboardText.setPosition(80.f, 180.f);

        pressText.setFont(font);
        pressText.setCharacterSize(20);
        pressText.setFillColor(sf::Color::Yellow);
        pressText.setString("Play again?  Y - Yes    N / ESC - Menu");
        sf::FloatRect pressBounds = pressText.getLocalBounds();
        pressText.setPosition(SETTINGS.SCREEN_WIDTH / 2.f - pressBounds.width / 2.f, SETTINGS.SCREEN_HEIGHT - 80.f);
    }

    void GameStateWin::UpdateLeaderboardText()
    {
        leaderboardText.setString(leaderboard.Format());
    }

    GameStateWin::GameStateWin(int score)
        : GameState(GameStateType::Win, true), finalScore(score)
    {
        InitText();
        finalScoreText.setString("YOUR SCORE: " + std::to_string(score));
        leaderboard.UpdatePlayerScore(score);
        UpdateLeaderboardText();
    }

    void GameStateWin::Update(float timeDelta) {}

    void GameStateWin::Draw(sf::RenderWindow& window)
    {
        window.draw(winText);
        window.draw(finalScoreText);
        window.draw(leaderboardText);
        window.draw(pressText);
    }

    void GameStateWin::HandleWindowEvent(const sf::Event& event)
    {
        if (event.type == sf::Event::KeyPressed)
        {
            switch (event.key.code)
            {
            case sf::Keyboard::Y:
                GetGame().StartGame();
                break;
            case sf::Keyboard::N:
            case sf::Keyboard::Escape:
                GetGame().ReturnToMenu();
                break;
            default: break;
            }
        }
    }
}