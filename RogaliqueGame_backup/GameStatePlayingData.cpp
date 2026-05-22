#include "GameStatePlayingData.h"

#include "GlassBlock.h"
#include "GameSettings.h"
#include "Game.h"
#include <iostream>

namespace RoqaliqueGame
{
    Game& GetGame();

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
        controlsText.setString("< A / D >  move    ESC pause");

        levelNameText.setFont(font);
        levelNameText.setCharacterSize(14);
        levelNameText.setFillColor(sf::Color(150, 150, 150));
        levelNameText.setPosition(SETTINGS.SCREEN_WIDTH / 2.f - 100.f, 5.f);
    }

    std::unique_ptr<Block> GameStatePlayingData::CreateBlockByType(BlockType type, const sf::Vector2f& position)
    {
        switch (type)
        {
        case BlockType::Simple:
            return std::make_unique<Block>(position);
        case BlockType::ThreeHit:
            return std::make_unique<ToughBlock>(position);
        case BlockType::Indestructible:
            return std::make_unique<IndestructibleBlock>(position);
        case BlockType::Magic:
        {
            auto block = std::make_unique<MagicBlock>(position);
            SetGlobalBlocksPtr(&blocks_);
            return block;
        }
        case BlockType::Glass:
            return std::make_unique<GlassBlock>(position);
        case BlockType::SmoothDestroy:
            return std::make_unique<SmoothDestroyableBlock>(position);
        default:
            return nullptr;
        }
    }

    void GameStatePlayingData::LoadLevel(int levelNumber)
    {
        blocks_.clear();

        if (levelNumber >= levelLoader.GetLevelCount())
        {
            std::cerr << "[LoadLevel] Invalid level number: " << levelNumber << std::endl;
            return;
        }

        Level& level = levelLoader.GetLevel(levelNumber);

        const float blockW = SETTINGS.BLOCK_WIDTH;
        const float blockH = SETTINGS.BLOCK_HEIGHT;
        const float padding = SETTINGS.BLOCK_PADDING;

        float totalBlocksWidth = level.width * blockW + (level.width - 1) * padding;
        float startX = (SETTINGS.SCREEN_WIDTH - totalBlocksWidth) / 2.0f;
        float startY = SETTINGS.BLOCK_START_Y;

        int blockCount = 0;

        for (const auto& blockInfo : level.blocks)
        {
            float x = startX + blockInfo.position.y * (blockW + padding);
            float y = startY + blockInfo.position.x * (blockH + padding);
            sf::Vector2f center(x + blockW / 2.0f, y + blockH / 2.0f);

            auto block = CreateBlockByType(blockInfo.type, center);
            if (block)
            {
                auto* indestructible = dynamic_cast<IndestructibleBlock*>(block.get());
                if (!indestructible)
                {
                    block->onDestroyed.AddListener(
                        [this, blockCenter = center](GameEvent event, int value) {
                            if (event == GameEvent::BlockDestroyed)
                            {
                                score += value;
                                scoreText.setString("Score: " + std::to_string(score));
                                bonusSpawner.TrySpawnBonus(blockCenter);
                                blockCounter->OnBlockDestroyed();
                            }
                        }
                    );
                    blockCount++;
                }
                blocks_.push_back(std::move(block));
            }
        }

        blockCounter->SetTotalBlocks(blockCount);
        levelNameText.setString("Level: " + level.name);
    }

    void GameStatePlayingData::NextLevel()
    {
        currentLevel++;

        if (currentLevel < levelLoader.GetLevelCount())
        {
            LoadLevel(currentLevel);
            ball_->Reset();
            ball_->ClearFallenFlag();
            platform_->SetPosition(sf::Vector2f(SETTINGS.SCREEN_WIDTH / 2.0f, SETTINGS.SCREEN_HEIGHT - 60.0f));
        }
        else
        {
            GetGame().WinGame(score);
        }
    }

    void GameStatePlayingData::CreateTestLevel()
    {
        blocks_.clear();

        const float blockW = SETTINGS.BLOCK_WIDTH;
        const float blockH = SETTINGS.BLOCK_HEIGHT;
        const float padding = SETTINGS.BLOCK_PADDING;

        float totalBlocksWidth = 8 * blockW + (8 - 1) * padding;
        float startX = (SETTINGS.SCREEN_WIDTH - totalBlocksWidth) / 2.0f;
        float startY = SETTINGS.BLOCK_START_Y;

        int blockCount = 0;

        for (int row = 0; row < 3; row++)
        {
            for (int col = 0; col < 8; col++)
            {
                float x = startX + col * (blockW + padding);
                float y = startY + row * (blockH + padding);
                sf::Vector2f center(x + blockW / 2.0f, y + blockH / 2.0f);

                auto block = std::make_unique<Block>(center);
                block->onDestroyed.AddListener(
                    [this, blockCenter = center](GameEvent event, int value) {
                        if (event == GameEvent::BlockDestroyed)
                        {
                            score += value;
                            scoreText.setString("Score: " + std::to_string(score));
                        }
                    }
                );
                blocks_.push_back(std::move(block));
                blockCount++;
            }
        }

        blockCounter->SetTotalBlocks(blockCount);
        levelNameText.setString("Level: TEST LEVEL");
    }

    void GameStatePlayingData::SetupBlockCounter()
    {
        blockCounter = std::make_shared<BlockCounter>([this]() { OnAllBlocksDestroyed(); });
    }

    void GameStatePlayingData::OnAllBlocksDestroyed()
    {
        bonusSpawner.Clear();
        if (currentLevel + 1 < levelLoader.GetLevelCount())
            NextLevel();
        else
            GetGame().WinGame(score);
    }

    void GameStatePlayingData::ApplyBonus(BonusType type)
    {
        switch (type)
        {
        case BonusType::FireBall:
            if (!fireBallStrategy || !fireBallStrategy->IsActive())
            {
                fireBallStrategy = std::make_unique<FireBallStrategy>();
                fireBallStrategy->ApplyEffect(ball_, platform_);
            }
            break;
        case BonusType::BigPlatform:
            if (!bigPlatformStrategy || !bigPlatformStrategy->IsActive())
            {
                bigPlatformStrategy = std::make_unique<BigPlatformStrategy>();
                bigPlatformStrategy->ApplyEffect(ball_, platform_);
            }
            break;
        case BonusType::DestroyAll:
            isDestroyAllMode = true;
            destroyAllTimer = 3.0f;
            break;
        }
    }

    void GameStatePlayingData::UpdateBonuses(float timeDelta)
    {
        if (fireBallStrategy && fireBallStrategy->IsActive())
            fireBallStrategy->Update(timeDelta);
        if (bigPlatformStrategy && bigPlatformStrategy->IsActive())
            bigPlatformStrategy->Update(timeDelta);
        if (isDestroyAllMode)
        {
            destroyAllTimer -= timeDelta;
            if (destroyAllTimer <= 0)
                isDestroyAllMode = false;
        }
    }

    GameStatePlayingData::GameStatePlayingData(int startLevel, int startScore, int startLives)
        : currentLevel(startLevel), score(startScore), lives(startLives)
    {
        platform_ = std::make_unique<Platform>(sf::Vector2f(SETTINGS.SCREEN_WIDTH / 2.0f, SETTINGS.SCREEN_HEIGHT - 60.0f));
        ball_ = std::make_unique<Ball>(sf::Vector2f(SETTINGS.SCREEN_WIDTH / 2.0f, SETTINGS.SCREEN_HEIGHT / 2.0f));

        InitText();
        scoreText.setString("Score: " + std::to_string(score));
        livesText.setString("Lives: " + std::to_string(lives));
        SetupBlockCounter();

        if (levelLoader.GetLevelCount() > 0 && currentLevel < levelLoader.GetLevelCount())
            LoadLevel(currentLevel);
        else if (levelLoader.GetLevelCount() > 0)
        {
            currentLevel = 0;
            LoadLevel(currentLevel);
        }
        else
            CreateTestLevel();
    }

    void GameStatePlayingData::SetGameParams(int level, int savedScore, int savedLives)
    {
        currentLevel = level;
        score = savedScore;
        lives = savedLives;
        scoreText.setString("Score: " + std::to_string(score));
        livesText.setString("Lives: " + std::to_string(lives));
    }

    void GameStatePlayingData::RestartLevel()
    {
        int savedLives = lives;
        int savedScore = score;

        blocks_.clear();
        ball_->Reset();
        ball_->ClearFallenFlag();
        platform_->SetPosition(sf::Vector2f(SETTINGS.SCREEN_WIDTH / 2.0f, SETTINGS.SCREEN_HEIGHT - 60.0f));
        LoadLevel(currentLevel);

        lives = savedLives;
        score = savedScore;
        livesText.setString("Lives: " + std::to_string(lives));
        scoreText.setString("Score: " + std::to_string(score));
    }

    void GameStatePlayingData::Update(float timeDelta)
    {
        UpdateBonuses(timeDelta);

        platform_->Update(timeDelta);
        ball_->Update(timeDelta);
        platform_->CheckCollision(*ball_);

        for (auto& block : blocks_)
        {
            block->Update(timeDelta);
            block->CheckCollision(*ball_);
        }

        bonusSpawner.Update(timeDelta);
        bonusSpawner.CheckCollisionWithPlatform(platform_->GetRect(), ball_, platform_,
            [this](BonusType type) { ApplyBonus(type); });

        if (ball_->HasFallen())
        {
            lives--;
            livesText.setString("Lives: " + std::to_string(lives));
            ball_->ClearFallenFlag();
            platform_->SetPosition(sf::Vector2f(SETTINGS.SCREEN_WIDTH / 2.0f, SETTINGS.SCREEN_HEIGHT - 60.0f));
            GetGame().SaveCurrentProgress(lives, score, currentLevel);
            if (lives <= 0)
                GetGame().LoseGame(score);
        }
    }

    void GameStatePlayingData::Draw(sf::RenderWindow& window)
    {
        platform_->Draw(window);
        ball_->Draw(window);
        for (const auto& block : blocks_) block->Draw(window);
        window.draw(scoreText);
        window.draw(livesText);
        window.draw(controlsText);
        window.draw(levelNameText);
        bonusSpawner.Draw(window);
    }
}
