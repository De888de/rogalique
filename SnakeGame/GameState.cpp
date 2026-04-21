#include <iostream>
#include "GameState.h"
#include "Platform.h"
#include "Ball.h"
#include "Block.h"
#include "ToughBlock.h"
#include "MagicBlock.h"
#include "SmoothDestroyableBlock.h"
#include "IndestructibleBlock.h"
#include "GameSettings.h"
#include "Game.h"
#include <string>
#include <random>
#include <chrono>

namespace ArkanoidGame
{
    Game& GetGame();
}

namespace ArkanoidGame
{
    // ====================== GameState ======================
    GameState::GameState(GameStateType type, bool isExclusivelyVisible)
        : type_(type), isExclusivelyVisible_(isExclusivelyVisible)
    {
    }

    // ====================== GameStatePlayingData ======================
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

    void GameStatePlayingData::InitBlocks()
    {
        blocks_.clear();

        const float blockW = SETTINGS.BLOCK_WIDTH;
        const float blockH = SETTINGS.BLOCK_HEIGHT;
        const float padding = SETTINGS.BLOCK_PADDING;
        const float startY = SETTINGS.BLOCK_START_Y;
        const int   rows = SETTINGS.BLOCK_ROWS;
        const int   cols = SETTINGS.BLOCK_COLUMNS;

        float totalBlocksWidth = cols * blockW + (cols - 1) * padding;
        float startX = (SETTINGS.SCREEN_WIDTH - totalBlocksWidth) / 2.0f;

        // Передаём указатель на список блоков для MagicBlock
        SetGlobalBlocksPtr(&blocks_);

        // Рандомная позиция для волшебного блока (не в первом ряду)
        static std::mt19937 gen(static_cast<unsigned>(std::chrono::steady_clock::now().time_since_epoch().count()));
        std::uniform_int_distribution<> rowDist(1, rows - 1);
        std::uniform_int_distribution<> colDist(1, cols - 2);  // не по краям, чтобы не пересекаться с железными

        int magicRow = rowDist(gen);
        int magicCol = colDist(gen);

        std::cout << "===== BLOCK CREATION =====" << std::endl;
        std::cout << "SCREEN_WIDTH: " << SETTINGS.SCREEN_WIDTH << std::endl;
        std::cout << "BLOCK_WIDTH: " << blockW << std::endl;
        std::cout << "BLOCK_COLUMNS: " << cols << std::endl;
        std::cout << "BLOCK_PADDING: " << padding << std::endl;
        std::cout << "totalBlocksWidth: " << totalBlocksWidth << std::endl;
        std::cout << "startX (left edge): " << startX << std::endl;
        std::cout << "First block center X: " << startX + blockW / 2 << std::endl;
        std::cout << "[MagicBlock] Placed at row " << magicRow << ", col " << magicCol << std::endl;

        for (int r = 0; r < rows; ++r)
        {
            for (int c = 0; c < cols; ++c)
            {
                float x = startX + c * (blockW + padding);
                float y = startY + r * (blockH + padding);
                sf::Vector2f center(x + blockW / 2, y + blockH / 2);

                std::unique_ptr<Block> block;

                // Железные блоки — по краям (первый и последний столбец)
                if (c == 0 || c == cols - 1)
                {
                    block = std::make_unique<IndestructibleBlock>(center);
                    std::cout << "[IndestructibleBlock] Created at (" << center.x << ", " << center.y << ")\n";
                }
                // Волшебный блок на случайной позиции
                else if (r == magicRow && c == magicCol)
                {
                    block = std::make_unique<MagicBlock>(center);
                    std::cout << "[MagicBlock] Created at (" << center.x << ", " << center.y << ")\n";
                }
                // Прочные блоки — верхний ряд
                else if (r == 0)
                {
                    block = std::make_unique<ToughBlock>(center);
                }
                // Обычные блоки — с плавным исчезновением
                else
                {
                    block = std::make_unique<SmoothDestroyableBlock>(center);
                }
                blocks_.push_back(std::move(block));
            }
        }
    }

    GameStatePlayingData::GameStatePlayingData()
    {
        platform_ = std::make_unique<Platform>(
            sf::Vector2f(SETTINGS.SCREEN_WIDTH / 2.0f, SETTINGS.SCREEN_HEIGHT - 60.0f));

        ball_ = std::make_unique<Ball>(
            sf::Vector2f(SETTINGS.SCREEN_WIDTH / 2.0f, SETTINGS.SCREEN_HEIGHT / 2.0f));

        InitBlocks();
        InitText();
    }

    bool GameStatePlayingData::IsWinCondition() const
    {
        // Проверяем, остались ли только неуничтожимые блоки
        for (const auto& block : blocks_)
        {
            // Если есть хоть один разрушаемый блок (не железный) и он активен — игра продолжается
            auto* indestructible = dynamic_cast<IndestructibleBlock*>(block.get());
            if (!indestructible && block->IsActive())
            {
                return false;  // есть ещё разрушаемые блоки
            }
        }
        return true;  // все разрушаемые блоки уничтожены
    }

    void GameStatePlayingData::Update(float timeDelta)
    {
        platform_->Update(timeDelta);
        ball_->Update(timeDelta);

        platform_->CheckCollision(*ball_);

        // Обновляем все блоки (для анимации SmoothDestroyableBlock)
        for (auto& block : blocks_)
        {
            block->Update(timeDelta);
        }

        // Цикл обработки столкновений с блоками (максимум 5 итераций)
        const int MAX_ITER = 5;
        int iter = 0;
        bool anyCollision = true;

        while (anyCollision && iter < MAX_ITER)
        {
            anyCollision = false;
            for (auto& block : blocks_)
            {
                // CheckCollision возвращает true только если блок уничтожен
                if (block->CheckCollision(*ball_))
                {
                    score += block->GetScoreValue();
                    scoreText.setString("Score: " + std::to_string(score));
                    anyCollision = true;
                    break;
                }
            }
            iter++;
        }

        if (ball_->HasFallen())
        {
            lives--;
            livesText.setString("Lives: " + std::to_string(lives));
            ball_->ClearFallenFlag();

            if (lives <= 0)
            {
                ArkanoidGame::GetGame().GoToGameOver(score);
                return;
            }
        }

        if (IsWinCondition())
        {
            ArkanoidGame::GetGame().GoToWin(score);
        }
    }

    void GameStatePlayingData::Draw(sf::RenderWindow& window)
    {
        platform_->Draw(window);
        ball_->Draw(window);

        for (const auto& block : blocks_)
        {
            block->Draw(window);
        }

        window.draw(scoreText);
        window.draw(livesText);
        window.draw(controlsText);
    }

    // ====================== GameStatePlaying ======================
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

    // ====================== GameStateGameOver ======================
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
        pressText.setString("Press R or SPACE to Restart\nESC to Exit");
        pressText.setPosition(SETTINGS.SCREEN_WIDTH / 2.f - 170.f, 340.f);
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
            switch (event.key.code)
            {
            case sf::Keyboard::R:
            case sf::Keyboard::Space:
                ArkanoidGame::GetGame().StartGame();
                break;
            case sf::Keyboard::Escape:
                ArkanoidGame::GetGame().ExitGame();
                break;
            }
        }
    }

    // ====================== GameStateWin ======================
    void GameStateWin::InitText()
    {
        if (!font.loadFromFile("Resources/Fonts/Roboto-Black.ttf"))
        {
            font.loadFromFile("D:/xyz/HW_03/xyz-cpp-course/ArkanoidGame/Resources/Fonts/Roboto-Black.ttf");
        }

        winText.setFont(font);
        winText.setCharacterSize(60);
        winText.setFillColor(sf::Color::Green);
        winText.setStyle(sf::Text::Bold);
        winText.setString("YOU WIN!");
        winText.setPosition(SETTINGS.SCREEN_WIDTH / 2.f - 170.f, 160.f);

        finalScoreText.setFont(font);
        finalScoreText.setCharacterSize(36);
        finalScoreText.setFillColor(sf::Color::White);
        finalScoreText.setString("Final Score: 0");
        finalScoreText.setPosition(SETTINGS.SCREEN_WIDTH / 2.f - 150.f, 250.f);

        pressText.setFont(font);
        pressText.setCharacterSize(24);
        pressText.setFillColor(sf::Color::Yellow);
        pressText.setString("Play again?  Y - Yes    N / ESC - No");
        pressText.setPosition(SETTINGS.SCREEN_WIDTH / 2.f - 180.f, 340.f);
    }

    GameStateWin::GameStateWin(int score)
        : GameState(GameStateType::Win, true), finalScore(score)
    {
        InitText();
        finalScoreText.setString("Final Score: " + std::to_string(score));
    }

    void GameStateWin::Update(float timeDelta) {}

    void GameStateWin::Draw(sf::RenderWindow& window)
    {
        window.draw(winText);
        window.draw(finalScoreText);
        window.draw(pressText);
    }

    void GameStateWin::HandleWindowEvent(const sf::Event& event)
    {
        if (event.type == sf::Event::KeyPressed)
        {
            switch (event.key.code)
            {
            case sf::Keyboard::Y:
                ArkanoidGame::GetGame().StartGame();
                break;
            case sf::Keyboard::N:
            case sf::Keyboard::Escape:
                ArkanoidGame::GetGame().ExitGame();
                break;
            }
        }
    }
}