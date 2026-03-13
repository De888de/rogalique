#include "Game.h"
#include "Constants.h"
#include "Rock.h"
#include <sstream>
#include <random>
#include <iostream>

namespace SnakeGame
{
    Game::Game()
        : snake(GridPosition(GRID_WIDTH / 2, GRID_HEIGHT / 2), INITIAL_SNAKE_LENGTH)
        , grid(GRID_WIDTH, GRID_HEIGHT)
        , menuManager(font, leaderboard)
    {
        Init();
    }

    void Game::Init()
    {
        // Загружаем шрифт
        std::string fontPath = FONTS_PATH + "arial.ttf";
        if (!font.loadFromFile(fontPath))
        {
            font.loadFromFile("C:/Windows/Fonts/arial.ttf");
        }

        // Загружаем текстуры
        LoadTextures();

        // Загружаем звуки
        soundManager.LoadSound("eat", SOUNDS_PATH + "AppleEat.wav");
        soundManager.LoadSound("death", SOUNDS_PATH + "Death.wav");
        soundManager.LoadMusic(SOUNDS_PATH + "background_music.ogg");

        // Звук волшебного яблока
        soundManager.LoadSound("magic", SOUNDS_PATH + "Magic.wav");

        // Звук телепортации через портал
        soundManager.LoadSound("portal", SOUNDS_PATH + "Portal.wav");

        // Создаем попап для звуков
        soundPopup = std::make_unique<SoundPopup>(font);

        SetupTexts();

        leaderboard.Load();
        soundManager.PlayMusic(true);
    }

    void Game::LoadTextures()
    {
        // Яблоко
        std::string applePath = RESOURCES_PATH + "apple.png";
        if (!appleTexture.loadFromFile(applePath))
        {
            applePath = RESOURCES_PATH + "Apple_M.png";
            if (!appleTexture.loadFromFile(applePath))
            {
                sf::Image img;
                img.create(GRID_CELL_SIZE, GRID_CELL_SIZE, sf::Color::Red);
                appleTexture.loadFromImage(img);
            }
        }

        // Волшебное яблоко
        std::string magicApplePath = RESOURCES_PATH + "MagicApple.png";
        if (!magicAppleTexture.loadFromFile(magicApplePath))
        {
            // Fallback
            sf::Image img;
            img.create(GRID_CELL_SIZE, GRID_CELL_SIZE, sf::Color(255, 100, 255));
            magicAppleTexture.loadFromImage(img);
        }

        // Змейка
        snake.LoadTextures(RESOURCES_PATH);

        // Камень 
        std::string rockPath = RESOURCES_PATH + "Rock.png";

        std::cout << "[Game] Loading rock from: " << rockPath << std::endl;
        Rock::LoadTexture(rockPath);
    }

    void Game::SetupTexts()
    {
        scoreText.setFont(font);
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setString("Score: 0");
        scoreText.setPosition(static_cast<float>(SCREEN_WIDTH - 150), 10.0f);

        gameOverText.setFont(font);
        gameOverText.setCharacterSize(50);
        gameOverText.setFillColor(sf::Color::Red);
        gameOverText.setString("GAME OVER");
        sf::FloatRect textRect = gameOverText.getLocalBounds();
        gameOverText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        gameOverText.setPosition(static_cast<float>(SCREEN_WIDTH) / 2.0f, static_cast<float>(SCREEN_HEIGHT) / 2.0f - 50.0f);

        finalScoreText.setFont(font);
        finalScoreText.setCharacterSize(30);
        finalScoreText.setFillColor(sf::Color::Yellow);

        menuHintText.setFont(font);
        menuHintText.setString("Menu: M | Pause: P | Sound: S");
        menuHintText.setCharacterSize(18);
        menuHintText.setFillColor(sf::Color(200, 200, 200));
        menuHintText.setPosition(10.0f, 10.0f);
    }

    // Вспомогательная функция расстояния
    int Game::GetManhattanDistance(const GridPosition& a, const GridPosition& b) const
    {
        return std::abs(a.x - b.x) + std::abs(a.y - b.y);
    }

    bool Game::IsFarFromObstacles(const GridPosition& pos, int minDist) const
    {
        for (const auto& rock : rocks)
        {
            if (GetManhattanDistance(pos, rock.GetPosition()) < minDist)
                return false;
        }

        if (GetManhattanDistance(pos, apple.GetPosition()) < minDist)
            return false;

        if (snake.CheckPositionCollision(pos))
            return false;

        if (pos.x < minDist || pos.x >= grid.GetWidth() - minDist ||
            pos.y < minDist || pos.y >= grid.GetHeight() - minDist)
            return false;

        return true;
    }

    void Game::Reset()
    {
        snake.Reset(GridPosition(GRID_WIDTH / 2, GRID_HEIGHT / 2), INITIAL_SNAKE_LENGTH);
        snake.SetDirection(SnakeDirection::Right);

        score = 0;
        moveTimer = 0.0f;
        scoreText.setString("Score: 0");

        ClearLevel();
        GenerateLevel();

        // Порталы только на INSANE и MAGICAL
        const auto& settings = GetDifficultySettings(m_currentDifficulty);
        if (settings.hasPortals)
        {
            GeneratePortals();
        }
        else
        {
            // Если порталов нет — ставим их в "невалидные" позиции, чтобы они не мешали
            portalPos1 = { -100, -100 };
            portalPos2 = { -100, -100 };
        }

        apple.Generate(grid, [this](const GridPosition& pos) {
            return snake.CheckPositionCollision(pos) || IsPositionOccupied(pos);
            });

        magicAppleActive = false;
        applesEatenSinceLastMagic = 0;
        magicApplePos = { -100, -100 };

        poisonAppleActive = false;
        poisonEffectTimer = 0.0f;
        isPoisoned = false;
        applesEatenSinceLastPoison = 0;
        poisonApplePos = { -100, -100 };


    }

    void Game::ClearLevel()
    {
        rocks.clear();
    }

    bool Game::IsPositionOccupied(const GridPosition& pos)
    {
        if (snake.CheckPositionCollision(pos))
            return true;

        for (const auto& rock : rocks)
            if (rock.GetPosition() == pos)
                return true;

        if (apple.GetPosition() == pos)
            return true;

        return false;
    }

    const DifficultySettings& Game::GetDifficultySettings(Difficulty diff) const
    {
        return DIFFICULTY_SETTINGS[static_cast<int>(diff)];
    }

    void Game::GenerateLevel()
    {
        const auto& settings = GetDifficultySettings(m_currentDifficulty);

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> posX(1, GRID_WIDTH - 2);
        std::uniform_int_distribution<> posY(1, GRID_HEIGHT - 2);

        // Генерируем камни
        for (int i = 0; i < settings.rockCount; ++i)
        {
            GridPosition pos;
            bool validPosition;
            int attempts = 0;

            do {
                validPosition = true;
                pos = { posX(gen), posY(gen) };

                if (IsPositionOccupied(pos))
                    validPosition = false;

                attempts++;
                if (attempts > 100) break;

            } while (!validPosition);

            if (validPosition)
                rocks.emplace_back(pos);
        }
    }

    void Game::CheckCollisions()
    {
        const auto& head = snake.GetHead();

        // Обычное яблоко
        if (head == apple.GetPosition())
        {
            score += POINTS_PER_APPLE;
            snake.Grow();
            scoreText.setString("Score: " + std::to_string(score));
            soundManager.PlaySound("eat");

            applesEatenSinceLastMagic++;
            applesEatenSinceLastPoison++;

            // Волшебное яблоко
            if (!magicAppleActive && applesEatenSinceLastMagic >= MAGIC_APPLE_MIN_INTERVAL)
            {
                if (applesEatenSinceLastMagic >= MAGIC_APPLE_MAX_INTERVAL || (rand() % 3 == 0))
                {
                    GenerateMagicApple();
                }
            }

            // Отравляющее яблоко
            if (!poisonAppleActive && applesEatenSinceLastPoison >= POISON_APPLE_MIN_INTERVAL)
            {
                if (applesEatenSinceLastPoison >= POISON_APPLE_MAX_INTERVAL || (rand() % 4 == 0))
                {
                    GeneratePoisonApple();
                }
            }

            apple.Generate(grid, [this](const GridPosition& pos) {
                return snake.CheckPositionCollision(pos) || IsPositionOccupied(pos) ||
                    (magicAppleActive && pos == magicApplePos) ||
                    (poisonAppleActive && pos == poisonApplePos);
                });
        }

        // Волшебное яблоко
        if (magicAppleActive && head == magicApplePos)
        {
            score += POINTS_PER_APPLE * 10;
            snake.Grow();
            scoreText.setString("Score: " + std::to_string(score));
            soundManager.PlaySound("magic");

            magicAppleActive = false;
            applesEatenSinceLastMagic = 0;
            snake.Grow();
        }

        // === ОТРАВЛЯЮЩЕЕ ЯБЛОКО ===
        if (poisonAppleActive && head == poisonApplePos)
        {
            score += POINTS_PER_APPLE;
            snake.Grow();
            scoreText.setString("Score: " + std::to_string(score));
            soundManager.PlaySound("eat");        // обычный звук

            poisonAppleActive = false;
            applesEatenSinceLastPoison = 0;

            // Активируем эффект отравления
            isPoisoned = true;
            poisonEffectTimer = POISON_DURATION;
        }

        // Камни
        for (const auto& rock : rocks)
        {
            if (head == rock.GetPosition())
            {
                GameOver();
                return;
            }
        }

        // Порталы (если включены)
        const auto& settings = GetDifficultySettings(m_currentDifficulty);
        if (settings.hasPortals)
        {
            if (head == portalPos1)
            {
                TeleportSnakeThroughPortal(portalPos1, portalPos2);
                return;
            }
            else if (head == portalPos2)
            {
                TeleportSnakeThroughPortal(portalPos2, portalPos1);
                return;
            }
        }

        // Столкновение с собой или стенами
        if (snake.CheckSelfCollision() || !grid.IsValidPosition(head))
        {
            if (justTeleported)
            {
                justTeleported = false;
                return;
            }
            GameOver();
        }
        else
        {
            justTeleported = false;
        }
    }
    

    void Game::GameOver()
    {
        state = GameState::GAME_OVER;
        soundManager.PlaySound("death");
        leaderboard.AddScore(score);
        leaderboard.Save();

        finalScoreText.setString("Your score: " + std::to_string(score));
        sf::FloatRect finalRect = finalScoreText.getLocalBounds();
        finalScoreText.setOrigin(finalRect.left + finalRect.width / 2.0f,
            finalRect.top + finalRect.height / 2.0f);
        finalScoreText.setPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f + 20.0f);
    }

    void Game::HandleInput()
    {
        static bool escPressed = false;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            if (!escPressed)
            {
                escPressed = true;
                switch (state)
                {
                case GameState::PLAYING:
                case GameState::PAUSED:
                    state = GameState::MAIN_MENU;
                    soundManager.PauseMusic();
                    break;
                case GameState::MAIN_MENU:
                    exit(0);
                    break;
                case GameState::DIFFICULTY_SELECT:
                case GameState::LEADERBOARD:
                    state = GameState::MAIN_MENU;
                    break;
                case GameState::SOUND_POPUP:
                    state = GameState::PLAYING;
                    break;
                default:
                    break;
                }
            }
        }
        else escPressed = false;

        static bool sPressed = false;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && !sPressed)
        {
            sPressed = true;
            if (state == GameState::PLAYING || state == GameState::PAUSED)
            {
                state = GameState::SOUND_POPUP;
                soundPopup->Reset();
                soundPopup->UpdateStates(soundManager.AreSoundsEnabled(),
                    soundManager.IsMusicEnabled());
            }
        }
        else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            sPressed = false;

        switch (state)
        {
        
        case GameState::DIFFICULTY_SELECT: HandleDifficultyInput(); break;
        case GameState::MAIN_MENU: HandleMainMenuInput(); break;

        case GameState::MENU: HandleGameMenuInput(); break;
        case GameState::PLAYING: HandlePlayingInput(); break;
        case GameState::SOUND_POPUP: HandleSoundPopupInput(); break;
        case GameState::PAUSED:
        {
            static bool pPressed = false;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::P) && !pPressed)
            {
                pPressed = true;
                state = GameState::PLAYING;
            }
            else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::P))
                pPressed = false;
        }
        break;
        default:
            break;
        }
    }

  void Game::HandleMainMenuInput()
{
    // Навигация
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !upPressed)
    {
        upPressed = true;
        menuManager.MoveMainMenuUp();
    }
    else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        upPressed = false;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !downPressed)
    {
        downPressed = true;
        menuManager.MoveMainMenuDown();
    }
    else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        downPressed = false;

    // Выбор пункта
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && !enterPressed)
    {
        enterPressed = true;

        switch (menuManager.GetSelectedMainItem())
        {
        case MainMenuItem::START_GAME:
            m_currentDifficulty = Difficulty::MEDIUM;
            menuManager.SetDifficulty(m_currentDifficulty);
            Reset();
            state = GameState::PLAYING;
            soundManager.PlayMusic(true);
           
            std::cout << "START GAME " << std::endl;
            break;

        case MainMenuItem::DIFFICULTY:
            state = GameState::DIFFICULTY_SELECT;
            menuManager.ResetDifficultySelection();
                     
            break;

        case MainMenuItem::LEADERBOARD:
            state = GameState::LEADERBOARD;
           
            break;

        case MainMenuItem::EXIT:
            exit(0);
            break;

        default:
            break;
        }
    }
    else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
        enterPressed = false;
}

    void Game::HandleDifficultyInput()
    {
       

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !upPressed)
        {
            upPressed = true;
            menuManager.MoveDifficultyUp();
        }
        else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            upPressed = false;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !downPressed)
        {
            downPressed = true;
            menuManager.MoveDifficultyDown();
        }
        else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            downPressed = false;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && !enterPressed)
        {
            enterPressed = true;

            m_currentDifficulty = menuManager.GetSelectedDifficulty();
            

            Reset();
            state = GameState::PLAYING;
            soundManager.PlayMusic(true);
            std::cout << "START DIFFICULTY" << std::endl;
           
        }
        else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            enterPressed = false;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && !escPressed)
        {
            escPressed = true;
            state = GameState::MAIN_MENU;
        }
        else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            escPressed = false;
    }

    void Game::HandleGameMenuInput()
    {
        

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !upPressed)
        {
            upPressed = true;
            menuManager.MoveGameMenuUp();
        }
        else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            upPressed = false;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !downPressed)
        {
            downPressed = true;
            menuManager.MoveGameMenuDown();
        }
        else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            downPressed = false;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && !enterPressed)
        {
            enterPressed = true;
            switch (menuManager.GetSelectedGameItem())
            {
            case GameMenuItem::CONTINUE:
                state = GameState::PLAYING;
                break;
            case GameMenuItem::RESTART:
                Reset();
                state = GameState::PLAYING;
                break;
            case GameMenuItem::LEADERBOARD:
                state = GameState::LEADERBOARD;
                break;
            case GameMenuItem::EXIT_TO_MENU:
                state = GameState::MAIN_MENU;
                soundManager.PauseMusic();
                break;
            default:
                break;
            }
        }
        else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            enterPressed = false;
    }

    void Game::HandlePlayingInput()
    {
       static bool mPressed = false, pPressed = false;
       static bool upPressed = false, downPressed = false, leftPressed = false, rightPressed = false;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::M) && !mPressed)
        {
            mPressed = true;
            state = GameState::MENU;
        }
        else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::M))
            mPressed = false;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::P) && !pPressed)
        {
            pPressed = true;
            state = GameState::PAUSED;
        }
        else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::P))
            pPressed = false;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !upPressed)
        {
            upPressed = true;
            if (snake.GetDirection() != SnakeDirection::Down)
                snake.SetDirection(SnakeDirection::Up);
        }
        else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            upPressed = false;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !downPressed)
        {
            downPressed = true;
            if (snake.GetDirection() != SnakeDirection::Up)
                snake.SetDirection(SnakeDirection::Down);
        }
        else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            downPressed = false;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !leftPressed)
        {
            leftPressed = true;
            if (snake.GetDirection() != SnakeDirection::Right)
                snake.SetDirection(SnakeDirection::Left);
        }
        else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            leftPressed = false;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !rightPressed)
        {
            rightPressed = true;
            if (snake.GetDirection() != SnakeDirection::Left)
                snake.SetDirection(SnakeDirection::Right);
        }
        else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            rightPressed = false;
    }

    void Game::HandleSoundPopupInput()
    {
        bool soundsEnabled = soundManager.AreSoundsEnabled();
        bool musicEnabled = soundManager.IsMusicEnabled();

        if (soundPopup->HandleInput(musicEnabled, soundsEnabled))
        {
            soundManager.SetSoundsEnabled(soundsEnabled);
            soundManager.SetMusicEnabled(musicEnabled);
            menuManager.UpdateSoundsState(soundsEnabled, musicEnabled);
        }
        else
        {
            state = GameState::PLAYING;
        }
    }

    void Game::Update(float deltaTime)
    {
        if (state != GameState::PLAYING) return;

        const auto& settings = GetDifficultySettings(m_currentDifficulty);

        // Обработка эффекта отравления
        if (isPoisoned)
        {
            poisonEffectTimer -= deltaTime;
            if (poisonEffectTimer <= 0.0f)
            {
                isPoisoned = false;
            }
        }

        moveTimer += deltaTime;

        float currentSpeed = settings.speed;
        if (isPoisoned)
            currentSpeed *= 2.0f;        // ускоряем в 2 раза

        float moveInterval = 1.0f / currentSpeed;

        while (moveTimer >= moveInterval)
        {
            moveTimer -= moveInterval;
            snake.Move();
            CheckCollisions();
        }
    }

    void Game::DrawGame(sf::RenderWindow& window, float deltaTime)
    {
        // ==================== СЕТКА ====================
        sf::RectangleShape cell(sf::Vector2f(static_cast<float>(GRID_CELL_SIZE - 1),
            static_cast<float>(GRID_CELL_SIZE - 1)));
        cell.setFillColor(sf::Color(50, 50, 50));
        for (int x = 0; x < grid.GetWidth(); ++x)
            for (int y = 0; y < grid.GetHeight(); ++y)
            {
                cell.setPosition(static_cast<float>(x * GRID_CELL_SIZE),
                    static_cast<float>(y * GRID_CELL_SIZE));
                window.draw(cell);
            }

        // ==================== КАМНИ ====================
        for (const auto& rock : rocks)
        {
            rock.Draw(window);
        }

        // ==================== ОБЫЧНОЕ ЯБЛОКО ====================
        sf::Sprite appleSprite;
        appleSprite.setTexture(appleTexture);
        float appleScaleX = static_cast<float>(GRID_CELL_SIZE) / static_cast<float>(appleTexture.getSize().x);
        float appleScaleY = static_cast<float>(GRID_CELL_SIZE) / static_cast<float>(appleTexture.getSize().y);
        appleSprite.setScale(appleScaleX, appleScaleY);
        appleSprite.setOrigin(static_cast<float>(appleTexture.getSize().x) / 2.0f,
            static_cast<float>(appleTexture.getSize().y) / 2.0f);
        appleSprite.setPosition(
            static_cast<float>(apple.GetPosition().x * GRID_CELL_SIZE + GRID_CELL_SIZE / 2),
            static_cast<float>(apple.GetPosition().y * GRID_CELL_SIZE + GRID_CELL_SIZE / 2));
        window.draw(appleSprite);


        // Волшебное яблоко
        if (magicAppleActive)
        {
            sf::Sprite magicSprite;
            magicSprite.setTexture(magicAppleTexture);
            float scaleX = static_cast<float>(GRID_CELL_SIZE) / magicAppleTexture.getSize().x;
            float scaleY = static_cast<float>(GRID_CELL_SIZE) / magicAppleTexture.getSize().y;
            magicSprite.setScale(scaleX, scaleY);
            magicSprite.setOrigin(static_cast<float>(magicAppleTexture.getSize().x) / 2.0f,
                static_cast<float>(magicAppleTexture.getSize().y) / 2.0f);
            magicSprite.setPosition(
                static_cast<float>(magicApplePos.x * GRID_CELL_SIZE + GRID_CELL_SIZE / 2),
                static_cast<float>(magicApplePos.y * GRID_CELL_SIZE + GRID_CELL_SIZE / 2)
            );
            window.draw(magicSprite);
        }

        // Отравляющее яблоко (кислотно-зелёный)
        if (poisonAppleActive)
        {
            sf::Sprite poisonSprite;
            poisonSprite.setTexture(appleTexture);                    // используем текстуру обычного яблока
            float scaleX = static_cast<float>(GRID_CELL_SIZE) / appleTexture.getSize().x;
            float scaleY = static_cast<float>(GRID_CELL_SIZE) / appleTexture.getSize().y;
            poisonSprite.setScale(scaleX, scaleY);
            poisonSprite.setOrigin(static_cast<float>(appleTexture.getSize().x) / 2.0f,
                static_cast<float>(appleTexture.getSize().y) / 2.0f);
            poisonSprite.setPosition(
                static_cast<float>(poisonApplePos.x * GRID_CELL_SIZE + GRID_CELL_SIZE / 2),
                static_cast<float>(poisonApplePos.y * GRID_CELL_SIZE + GRID_CELL_SIZE / 2)
            );
            poisonSprite.setColor(sf::Color(279, 255, 30));     
            window.draw(poisonSprite);
        }

        // ==================== ЗМЕЙКА  ====================
        const auto& segments = snake.GetSegments();
        for (size_t i = 0; i < segments.size(); ++i)
        {
            sf::Texture* texture = snake.GetSegmentTexture(i);
            if (!texture)
            {
                sf::RectangleShape fallbackRect(sf::Vector2f(static_cast<float>(GRID_CELL_SIZE - 2),
                    static_cast<float>(GRID_CELL_SIZE - 2)));
                if (i == 0) fallbackRect.setFillColor(sf::Color::Red);
                else if (i == segments.size() - 1) fallbackRect.setFillColor(sf::Color::Blue);
                else fallbackRect.setFillColor(sf::Color::Green);
                fallbackRect.setPosition(static_cast<float>(segments[i].x * GRID_CELL_SIZE + 1),
                    static_cast<float>(segments[i].y * GRID_CELL_SIZE + 1));
                window.draw(fallbackRect);
                continue;
            }
            sf::Sprite segmentSprite;
            segmentSprite.setTexture(*texture);

            float scaleX = static_cast<float>(GRID_CELL_SIZE) / static_cast<float>(texture->getSize().x);
            float scaleY = static_cast<float>(GRID_CELL_SIZE) / static_cast<float>(texture->getSize().y);
            segmentSprite.setScale(scaleX, scaleY);

            segmentSprite.setOrigin(static_cast<float>(texture->getSize().x) / 2.0f,
                static_cast<float>(texture->getSize().y) / 2.0f);

            segmentSprite.setPosition(
                static_cast<float>(segments[i].x * GRID_CELL_SIZE + GRID_CELL_SIZE / 2),
                static_cast<float>(segments[i].y * GRID_CELL_SIZE + GRID_CELL_SIZE / 2)
            );

            // Поворот сегмента 
            float rotation = 0.0f;
            if (i == 0) // Голова
            {
                switch (snake.GetDirection())
                {
                case SnakeDirection::Up:    rotation = 0.0f; break;
                case SnakeDirection::Right: rotation = 90.0f; break;
                case SnakeDirection::Down:  rotation = 180.0f; break;
                case SnakeDirection::Left:  rotation = 270.0f; break;
                }
            }
            else if (i == segments.size() - 1) // Хвост
            {
                const auto& prev = segments[i - 1];
                const auto& curr = segments[i];

                if (prev.x < curr.x) rotation = 270.0f;
                else if (prev.x > curr.x) rotation = 90.0f;
                else if (prev.y < curr.y) rotation = 0.0f;
                else if (prev.y > curr.y) rotation = 180.0f;
            }
            else // Тело
            {
                const auto& prev = segments[i - 1];
                const auto& curr = segments[i];
                const auto& next = segments[i + 1];

                if (prev.x == curr.x && curr.x == next.x) rotation = 0.0f;
                else if (prev.y == curr.y && curr.y == next.y) rotation = 90.0f;
                else
                {
                    int inX = curr.x - prev.x;
                    int inY = curr.y - prev.y;
                    int outX = next.x - curr.x;
                    int outY = next.y - curr.y;

                    if (inX == 0 && inY == -1 && outX == 1 && outY == 0) rotation = 180.0f;
                    else if (inX == 1 && inY == 0 && outX == 0 && outY == 1) rotation = 270.0f;
                    else if (inX == 0 && inY == 1 && outX == -1 && outY == 0) rotation = 0.0f;
                    else if (inX == -1 && inY == 0 && outX == 0 && outY == -1) rotation = 90.0f;
                    else if (inX == 0 && inY == -1 && outX == -1 && outY == 0) rotation = 270.0f;
                    else if (inX == -1 && inY == 0 && outX == 0 && outY == 1) rotation = 180.0f;
                    else if (inX == 0 && inY == 1 && outX == 1 && outY == 0) rotation = 90.0f;
                    else if (inX == 1 && inY == 0 && outX == 0 && outY == -1) rotation = 0.0f;
                }
            }
            segmentSprite.setRotation(rotation);
            window.draw(segmentSprite);
        }

            // ==================== ПОРТАЛЫ С АНИМАЦИЕЙ ====================
        {
            const float cellSize = static_cast<float>(GRID_CELL_SIZE);
            const float baseRadius = cellSize / 2.0f - 5.0f;

            // Плавное уменьшение подложки обратно к нормальному размеру
            if (portalGlowScale > portalGlowTargetScale)
            {
                portalGlowScale -= portalGlowSpeed * deltaTime;  
                if (portalGlowScale < portalGlowTargetScale)
                    portalGlowScale = portalGlowTargetScale;
            }

            // Белая подложка
            float currentGlowRadius = (baseRadius + 12.0f) * portalGlowScale;
            sf::CircleShape whiteGlow(currentGlowRadius);
            whiteGlow.setOrigin(currentGlowRadius, currentGlowRadius);
            whiteGlow.setFillColor(sf::Color(255, 255, 255, 230));

            // Основной портал
            portalShape.setRadius(baseRadius);
            portalShape.setOrigin(baseRadius, baseRadius);
            portalShape.setFillColor(sf::Color(0, 210, 255));
            portalShape.setOutlineThickness(4.0f);
            portalShape.setOutlineColor(sf::Color::White);

            // Первый портал
            sf::Vector2f pos1(portalPos1.x * cellSize + cellSize / 2.0f,
                portalPos1.y * cellSize + cellSize / 2.0f);
            whiteGlow.setPosition(pos1);
            window.draw(whiteGlow);
            portalShape.setPosition(pos1);
            window.draw(portalShape);

            // Второй портал
            sf::Vector2f pos2(portalPos2.x * cellSize + cellSize / 2.0f,
                portalPos2.y * cellSize + cellSize / 2.0f);
            whiteGlow.setPosition(pos2);
            window.draw(whiteGlow);
            portalShape.setPosition(pos2);
            window.draw(portalShape);
        }
        // ==================== ТЕКСТЫ ====================
        window.draw(scoreText);
        window.draw(menuHintText);
    }
    void Game::ResetInputFlags()
    {
        upPressed = false;
        downPressed = false;
        enterPressed = false;
        escPressed = false;
        sPressed = false;
        pPressed = false;
        mPressed = false;
    }
    void Game::Draw(sf::RenderWindow& window)
    {
        window.clear(sf::Color::Black);

        switch (state)
        {
        case GameState::MAIN_MENU:
            menuManager.DrawMainMenu(window);
            break;

        case GameState::DIFFICULTY_SELECT:
            menuManager.DrawDifficultyMenu(window, m_currentDifficulty);
            break;

        case GameState::LEADERBOARD:
            menuManager.DrawLeaderboard(window);
            break;

        case GameState::MENU:
            DrawGame(window, 0.0f);                    // меню паузы
            menuManager.DrawGameMenu(window);
            break;

        case GameState::SOUND_POPUP:
            DrawGame(window, 0.0f);
            soundPopup->Draw(window);
            break;

        case GameState::PLAYING:
        case GameState::PAUSED:
            DrawGame(window, 1.0f / 60.0f);            
            if (state == GameState::PAUSED)
            {
                sf::RectangleShape pauseOverlay(sf::Vector2f(static_cast<float>(SCREEN_WIDTH),
                    static_cast<float>(SCREEN_HEIGHT)));
                pauseOverlay.setFillColor(sf::Color(0, 0, 0, 150));
                window.draw(pauseOverlay);

                sf::Text pauseText;
                pauseText.setFont(font);
                pauseText.setString("PAUSED\nPress P to continue");
                pauseText.setCharacterSize(48);
                pauseText.setFillColor(sf::Color::Yellow);
                sf::FloatRect pauseRect = pauseText.getLocalBounds();
                pauseText.setOrigin(pauseRect.left + pauseRect.width / 2.0f,
                    pauseRect.top + pauseRect.height / 2.0f);
                pauseText.setPosition(static_cast<float>(SCREEN_WIDTH) / 2.0f,
                    static_cast<float>(SCREEN_HEIGHT) / 2.0f);
                window.draw(pauseText);
            }
            break;

        case GameState::GAME_OVER:
        {
            // Отрисовка игрового поля (чтобы был виден финальный кадр)
            DrawGame(window, 0.0f);

            // Полупрозрачный оверлей
            sf::RectangleShape overlay(sf::Vector2f(static_cast<float>(SCREEN_WIDTH),
                static_cast<float>(SCREEN_HEIGHT)));
            overlay.setFillColor(sf::Color(0, 0, 0, 200));
            window.draw(overlay);

            // Текст GAME OVER
            window.draw(gameOverText);
            window.draw(finalScoreText);

            // Подсказка
            sf::Text continueText;
            continueText.setFont(font);
            continueText.setString("Press SPACE to continue");
            continueText.setCharacterSize(20);
            continueText.setFillColor(sf::Color::White);
            sf::FloatRect continueRect = continueText.getLocalBounds();
            continueText.setOrigin(continueRect.left + continueRect.width / 2.0f,
                continueRect.top + continueRect.height / 2.0f);
            continueText.setPosition(static_cast<float>(SCREEN_WIDTH) / 2.0f,
                static_cast<float>(SCREEN_HEIGHT) - 50.0f);
            window.draw(continueText);

            // Обработка нажатия SPACE
            static bool spacePressed = false;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            {
                if (!spacePressed)
                {
                    spacePressed = true;
                    state = GameState::MAIN_MENU;
                }
            }
            else
                spacePressed = false;
            break;
        }
        }
 
    }
    void Game::GeneratePortals()
    {
        const int minDistanceToObstacles = 3;
        const int minDistanceBetweenPortals = 9;

        int attempts = 0;
        const int maxAttempts = 500;

        while (attempts < maxAttempts)
        {
            attempts++;

            do {
                portalPos1 = grid.GetRandomPosition();
            } while (IsPositionOccupied(portalPos1) || !IsFarFromObstacles(portalPos1, minDistanceToObstacles));

            do {
                portalPos2 = grid.GetRandomPosition();
            } while (IsPositionOccupied(portalPos2) ||
                !IsFarFromObstacles(portalPos2, minDistanceToObstacles) ||
                GetManhattanDistance(portalPos1, portalPos2) < minDistanceBetweenPortals);

            if (GetManhattanDistance(portalPos1, portalPos2) >= minDistanceBetweenPortals)
                return;
        }

        // Fallback
        portalPos1 = { GRID_WIDTH / 4, GRID_HEIGHT / 2 };
        portalPos2 = { GRID_WIDTH * 3 / 4, GRID_HEIGHT / 2 };
    }

    void Game::TeleportSnakeThroughPortal(const GridPosition& from, const GridPosition& to)
    {
        SnakeDirection dir = snake.GetDirection();

        snake.TeleportHead(to);

        GridPosition exitPos = to;
        switch (dir)
        {
        case SnakeDirection::Up:    exitPos.y -= 1; break;
        case SnakeDirection::Down:  exitPos.y += 1; break;
        case SnakeDirection::Left:  exitPos.x -= 1; break;
        case SnakeDirection::Right: exitPos.x += 1; break;
        }

        if (grid.IsValidPosition(exitPos) && !snake.CheckPositionCollision(exitPos))
        {
            snake.TeleportHead(exitPos);
        }
        snake.SetDirection(dir);

        // Анимация белой подложки
        portalGlowScale = 2.3f;
        portalGlowTargetScale = 1.0f;

        justTeleported = true;

        // Звук телепортации 
        soundManager.PlaySound("portal");
    }
    void Game::GenerateMagicApple()
    {
        const auto& settings = GetDifficultySettings(m_currentDifficulty);
        if (!settings.hasMagicApples)
            return;

        int attempts = 0;
        const int maxAttempts = 100;

        do {
            magicApplePos = grid.GetRandomPosition();
            attempts++;
        } while ((IsPositionOccupied(magicApplePos) ||
            GetManhattanDistance(magicApplePos, apple.GetPosition()) < 4)
            && attempts < maxAttempts);

        if (attempts < maxAttempts)
            magicAppleActive = true;
        else
            magicAppleActive = false;   // если не нашли место — не показываем
    }

    void Game::GeneratePoisonApple()
    {
        int attempts = 0;
        const int maxAttempts = 100;

        do {
            poisonApplePos = grid.GetRandomPosition();
            attempts++;
        } while ((IsPositionOccupied(poisonApplePos) ||
            GetManhattanDistance(poisonApplePos, apple.GetPosition()) < 3 ||
            (magicAppleActive && GetManhattanDistance(poisonApplePos, magicApplePos) < 3) ||
            (poisonAppleActive && GetManhattanDistance(poisonApplePos, poisonApplePos) < 3))
            && attempts < maxAttempts);

        poisonAppleActive = (attempts < maxAttempts);
    }

}