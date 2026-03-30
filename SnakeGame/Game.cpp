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
        if (!font.loadFromFile(FONTS_PATH + "arial.ttf"))
            font.loadFromFile("C:/Windows/Fonts/arial.ttf");

        LoadTextures();

        soundManager.LoadSound("eat", SOUNDS_PATH + "AppleEat.wav");
        soundManager.LoadSound("death", SOUNDS_PATH + "Death.wav");
        soundManager.LoadSound("magic", SOUNDS_PATH + "Magic.wav");
        soundManager.LoadSound("portal", SOUNDS_PATH + "Portal.wav");
        soundManager.LoadMusic(SOUNDS_PATH + "background_music.ogg");

        soundPopup = std::make_unique<SoundPopup>(font);
        SetupTexts();
        leaderboard.Load();
        soundManager.PlayMusic(true);
    }

    void Game::LoadTextures()
    {
        auto loadTex = [&](sf::Texture& tex, const std::string& path, sf::Color fallback) {
            if (!tex.loadFromFile(path)) {
                sf::Image img;
                img.create(GRID_CELL_SIZE, GRID_CELL_SIZE, fallback);
                tex.loadFromImage(img);
            }
            };
        loadTex(appleTexture, RESOURCES_PATH + "apple.png", sf::Color::Red);
        loadTex(magicAppleTexture, RESOURCES_PATH + "MagicApple.png", sf::Color(255, 100, 255));
        snake.LoadTextures(RESOURCES_PATH);
        Rock::LoadTexture(RESOURCES_PATH + "Rock.png");
    }

    void Game::SetupTexts()
    {
        scoreText.setFont(font);
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setString("Score: 0");
        scoreText.setPosition(SCREEN_WIDTH - 150.f, 10.f);

        gameOverText.setFont(font);
        gameOverText.setCharacterSize(50);
        gameOverText.setFillColor(sf::Color::Red);
        gameOverText.setString("GAME OVER");
        gameOverText.setOrigin(gameOverText.getLocalBounds().width / 2, gameOverText.getLocalBounds().height / 2);
        gameOverText.setPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f - 50.f);

        finalScoreText.setFont(font);
        finalScoreText.setCharacterSize(30);
        finalScoreText.setFillColor(sf::Color::Yellow);

        menuHintText.setFont(font);
        menuHintText.setString("Menu: M | Pause: P | Sound: S");
        menuHintText.setCharacterSize(18);
        menuHintText.setFillColor(sf::Color(200, 200, 200));
        menuHintText.setPosition(10.f, 10.f);
    }

    int Game::GetManhattanDistance(const GridPosition& a, const GridPosition& b) const
    {
        return std::abs(a.x - b.x) + std::abs(a.y - b.y);
    }

    bool Game::IsFarFromObstacles(const GridPosition& pos, int minDist) const
    {
        for (const auto& rock : rocks)
            if (GetManhattanDistance(pos, rock.GetPosition()) < minDist) return false;
        if (GetManhattanDistance(pos, apple.GetPosition()) < minDist) return false;
        if (snake.CheckPositionCollision(pos)) return false;
        if (pos.x < minDist || pos.x >= grid.GetWidth() - minDist ||
            pos.y < minDist || pos.y >= grid.GetHeight() - minDist) return false;
        return true;
    }

    bool Game::IsPositionOccupied(const GridPosition& pos)
    {
        if (snake.CheckPositionCollision(pos)) return true;
        for (const auto& rock : rocks)
            if (rock.GetPosition() == pos) return true;
        return apple.GetPosition() == pos;
    }

    const DifficultySettings& Game::GetDifficultySettings(Difficulty diff) const
    {
        return DIFFICULTY_SETTINGS[static_cast<int>(diff)];
    }

    void Game::ClearLevel() { rocks.clear(); }

    void Game::GenerateLevel()
    {
        const auto& settings = GetDifficultySettings(m_currentDifficulty);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> posX(1, GRID_WIDTH - 2);
        std::uniform_int_distribution<> posY(1, GRID_HEIGHT - 2);
        for (int i = 0; i < settings.rockCount; ++i)
            for (int attempts = 0; attempts < 100; ++attempts) {
                GridPosition pos = { posX(gen), posY(gen) };
                if (!IsPositionOccupied(pos)) {
                    rocks.emplace_back(pos);
                    break;
                }
            }
    }

    void Game::Reset()
    {
        snake.Reset(GridPosition(GRID_WIDTH / 2, GRID_HEIGHT / 2), INITIAL_SNAKE_LENGTH);
        snake.SetDirection(SnakeDirection::Right);
        score = 0;
        moveTimer = 0.f;
        scoreText.setString("Score: 0");

        ClearLevel();
        GenerateLevel();

        const auto& settings = GetDifficultySettings(m_currentDifficulty);
        if (settings.hasPortals) GeneratePortals();
        else portalPos1 = portalPos2 = { -100, -100 };

        apple.Generate(grid, [this](const GridPosition& pos) {
            return snake.CheckPositionCollision(pos) || IsPositionOccupied(pos);
            });

        magicAppleActive = poisonAppleActive = false;
        applesEatenSinceLastMagic = applesEatenSinceLastPoison = 0;
        isPoisoned = false;
        poisonEffectTimer = 0.f;
        magicApplePos = poisonApplePos = { -100, -100 };
    }

    void Game::GenerateMagicApple()
    {
        if (!GetDifficultySettings(m_currentDifficulty).hasMagicApples) return;
        for (int attempts = 0; attempts < 100; ++attempts) {
            magicApplePos = grid.GetRandomPosition();
            if (!IsPositionOccupied(magicApplePos) &&
                GetManhattanDistance(magicApplePos, apple.GetPosition()) >= 4) {
                magicAppleActive = true;
                return;
            }
        }
        magicAppleActive = false;
    }

    void Game::GeneratePoisonApple()
    {
        for (int attempts = 0; attempts < 100; ++attempts) {
            poisonApplePos = grid.GetRandomPosition();
            if (!IsPositionOccupied(poisonApplePos) &&
                GetManhattanDistance(poisonApplePos, apple.GetPosition()) >= 3 &&
                (!magicAppleActive || GetManhattanDistance(poisonApplePos, magicApplePos) >= 3)) {
                poisonAppleActive = true;
                return;
            }
        }
        poisonAppleActive = false;
    }

    void Game::GeneratePortals()
    {
        for (int attempts = 0; attempts < 500; ++attempts) {
            portalPos1 = grid.GetRandomPosition();
            if (!IsFarFromObstacles(portalPos1, 3)) continue;
            portalPos2 = grid.GetRandomPosition();
            if (IsFarFromObstacles(portalPos2, 3) &&
                GetManhattanDistance(portalPos1, portalPos2) >= 9) return;
        }
        portalPos1 = { GRID_WIDTH / 4, GRID_HEIGHT / 2 };
        portalPos2 = { GRID_WIDTH * 3 / 4, GRID_HEIGHT / 2 };
    }

    void Game::TeleportSnakeThroughPortal(const GridPosition& from, const GridPosition& to)
    {
        auto dir = snake.GetDirection();
        snake.TeleportHead(to);
        GridPosition exitPos = to;
        switch (dir) {
        case SnakeDirection::Up:    exitPos.y--; break;
        case SnakeDirection::Down:  exitPos.y++; break;
        case SnakeDirection::Left:  exitPos.x--; break;
        case SnakeDirection::Right: exitPos.x++; break;
        }
        if (grid.IsValidPosition(exitPos) && !snake.CheckPositionCollision(exitPos))
            snake.TeleportHead(exitPos);
        snake.SetDirection(dir);
        portalGlowScale = 2.3f;
        portalGlowTargetScale = 1.f;
        justTeleported = true;
        soundManager.PlaySound("portal");
    }

    void Game::CheckCollisions()
    {
        const auto& head = snake.GetHead();
        if (head == apple.GetPosition()) {
            score += POINTS_PER_APPLE;
            snake.Grow();
            scoreText.setString("Score: " + std::to_string(score));
            soundManager.PlaySound("eat");

            if (++applesEatenSinceLastMagic >= MAGIC_APPLE_MIN_INTERVAL)
                if (applesEatenSinceLastMagic >= MAGIC_APPLE_MAX_INTERVAL || rand() % 3 == 0)
                    GenerateMagicApple();
            if (++applesEatenSinceLastPoison >= POISON_APPLE_MIN_INTERVAL)
                if (applesEatenSinceLastPoison >= POISON_APPLE_MAX_INTERVAL || rand() % 4 == 0)
                    GeneratePoisonApple();

            apple.Generate(grid, [this](const GridPosition& pos) {
                return snake.CheckPositionCollision(pos) || IsPositionOccupied(pos) ||
                    (magicAppleActive && pos == magicApplePos) ||
                    (poisonAppleActive && pos == poisonApplePos);
                });
        }

        if (magicAppleActive && head == magicApplePos) {
            score += POINTS_PER_APPLE * 10;
            snake.Grow(); snake.Grow();
            scoreText.setString("Score: " + std::to_string(score));
            soundManager.PlaySound("magic");
            magicAppleActive = false;
            applesEatenSinceLastMagic = 0;
        }

        if (poisonAppleActive && head == poisonApplePos) {
            score += POINTS_PER_APPLE;
            snake.Grow();
            scoreText.setString("Score: " + std::to_string(score));
            soundManager.PlaySound("eat");
            poisonAppleActive = false;
            applesEatenSinceLastPoison = 0;
            isPoisoned = true;
            poisonEffectTimer = POISON_DURATION;
        }

        for (const auto& rock : rocks)
            if (head == rock.GetPosition()) { GameOver(); return; }

        const auto& settings = GetDifficultySettings(m_currentDifficulty);
        if (settings.hasPortals) {
            if (head == portalPos1) { TeleportSnakeThroughPortal(portalPos1, portalPos2); return; }
            if (head == portalPos2) { TeleportSnakeThroughPortal(portalPos2, portalPos1); return; }
        }

        if (snake.CheckSelfCollision() || !grid.IsValidPosition(head)) {
            if (!justTeleported) GameOver();
            else justTeleported = false;
        }
        else justTeleported = false;
    }

    void Game::GameOver()
    {
        state = GameState::GAME_OVER;
        soundManager.PlaySound("death");
        leaderboard.AddScore(score);
        leaderboard.Save();
        finalScoreText.setString("Your score: " + std::to_string(score));
        finalScoreText.setOrigin(finalScoreText.getLocalBounds().width / 2, finalScoreText.getLocalBounds().height / 2);
        finalScoreText.setPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f + 20.f);
    }

    void Game::Update(float deltaTime)
    {
        if (state != GameState::PLAYING) return;
        const auto& settings = GetDifficultySettings(m_currentDifficulty);
        if (isPoisoned) {
            poisonEffectTimer -= deltaTime;
            if (poisonEffectTimer <= 0.f) isPoisoned = false;
        }
        moveTimer += deltaTime;
        float moveInterval = 1.f / (settings.speed * (isPoisoned ? 2.f : 1.f));
        while (moveTimer >= moveInterval) {
            moveTimer -= moveInterval;
            snake.Move();
            CheckCollisions();
        }
    }

    // ==================== ОБРАБОТКА ВВОДА ====================
    void Game::HandleInput()
    {
        static bool escPressed = false;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            if (!escPressed) {
                escPressed = true;
                switch (state) {
                case GameState::PLAYING:
                case GameState::PAUSED: state = GameState::MAIN_MENU; soundManager.PauseMusic(); break;
                case GameState::MAIN_MENU: exit(0); break;
                case GameState::DIFFICULTY_SELECT:
                case GameState::LEADERBOARD: state = GameState::MAIN_MENU; break;
                case GameState::SOUND_POPUP: state = GameState::PLAYING; break;
                default: break;
                }
            }
        }
        else escPressed = false;

        static bool sPressed = false;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && !sPressed) {
            sPressed = true;
            if (state == GameState::PLAYING || state == GameState::PAUSED) {
                state = GameState::SOUND_POPUP;
                soundPopup->Reset();
                soundPopup->UpdateStates(soundManager.AreSoundsEnabled(), soundManager.IsMusicEnabled());
            }
        }
        else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::S)) sPressed = false;

        switch (state) {
        case GameState::DIFFICULTY_SELECT: HandleDifficultyInput(); break;
        case GameState::MAIN_MENU: HandleMainMenuInput(); break;
        case GameState::MENU: HandleGameMenuInput(); break;
        case GameState::PLAYING: HandlePlayingInput(); break;
        case GameState::SOUND_POPUP: HandleSoundPopupInput(); break;
        case GameState::PAUSED: {
            static bool pPressed = false;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::P) && !pPressed) {
                pPressed = true;
                state = GameState::PLAYING;
            }
            else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::P)) pPressed = false;
        } break;
        default: break;
        }
    }

    void Game::HandleMainMenuInput()
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !upPressed) {
            upPressed = true;
            menuManager.MoveMainMenuUp();
        }
        else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) upPressed = false;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !downPressed) {
            downPressed = true;
            menuManager.MoveMainMenuDown();
        }
        else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) downPressed = false;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && !enterPressed) {
            enterPressed = true;
            switch (menuManager.GetSelectedMainItem()) {
            case MainMenuItem::START_GAME:
                m_currentDifficulty = Difficulty::MEDIUM;
                menuManager.SetDifficulty(m_currentDifficulty);
                Reset();
                state = GameState::PLAYING;
                soundManager.PlayMusic(true);
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
            default: break;
            }
        }
        else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) enterPressed = false;
    }

    void Game::HandleDifficultyInput()
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !upPressed) {
            upPressed = true;
            menuManager.MoveDifficultyUp();
        }
        else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) upPressed = false;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !downPressed) {
            downPressed = true;
            menuManager.MoveDifficultyDown();
        }
        else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) downPressed = false;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && !enterPressed) {
            enterPressed = true;
            m_currentDifficulty = menuManager.GetSelectedDifficulty();
            Reset();
            state = GameState::PLAYING;
            soundManager.PlayMusic(true);
        }
        else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) enterPressed = false;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && !escPressed) {
            escPressed = true;
            state = GameState::MAIN_MENU;
        }
        else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) escPressed = false;
    }

    void Game::HandleGameMenuInput()
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !upPressed) {
            upPressed = true;
            menuManager.MoveGameMenuUp();
        }
        else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) upPressed = false;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !downPressed) {
            downPressed = true;
            menuManager.MoveGameMenuDown();
        }
        else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) downPressed = false;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && !enterPressed) {
            enterPressed = true;
            switch (menuManager.GetSelectedGameItem()) {
            case GameMenuItem::CONTINUE: state = GameState::PLAYING; break;
            case GameMenuItem::RESTART: Reset(); state = GameState::PLAYING; break;
            case GameMenuItem::LEADERBOARD: state = GameState::LEADERBOARD; break;
            case GameMenuItem::EXIT_TO_MENU: state = GameState::MAIN_MENU; soundManager.PauseMusic(); break;
            default: break;
            }
        }
        else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) enterPressed = false;
    }

    void Game::HandlePlayingInput()
    {
        static bool mPressed = false, pPressed = false;
        static bool upPressed = false, downPressed = false, leftPressed = false, rightPressed = false;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::M) && !mPressed) {
            mPressed = true;
            state = GameState::MENU;
        }
        else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::M)) mPressed = false;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::P) && !pPressed) {
            pPressed = true;
            state = GameState::PAUSED;
        }
        else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::P)) pPressed = false;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !upPressed) {
            upPressed = true;
            if (snake.GetDirection() != SnakeDirection::Down) snake.SetDirection(SnakeDirection::Up);
        }
        else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) upPressed = false;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !downPressed) {
            downPressed = true;
            if (snake.GetDirection() != SnakeDirection::Up) snake.SetDirection(SnakeDirection::Down);
        }
        else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) downPressed = false;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !leftPressed) {
            leftPressed = true;
            if (snake.GetDirection() != SnakeDirection::Right) snake.SetDirection(SnakeDirection::Left);
        }
        else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) leftPressed = false;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !rightPressed) {
            rightPressed = true;
            if (snake.GetDirection() != SnakeDirection::Left) snake.SetDirection(SnakeDirection::Right);
        }
        else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) rightPressed = false;
    }

    void Game::HandleSoundPopupInput()
    {
        bool soundsEnabled = soundManager.AreSoundsEnabled();
        bool musicEnabled = soundManager.IsMusicEnabled();
        if (soundPopup->HandleInput(musicEnabled, soundsEnabled)) {
            soundManager.SetSoundsEnabled(soundsEnabled);
            soundManager.SetMusicEnabled(musicEnabled);
            menuManager.UpdateSoundsState(soundsEnabled, musicEnabled);
        }
        else {
            state = GameState::PLAYING;
        }
    }

    void Game::ResetInputFlags()
    {
        upPressed = downPressed = enterPressed = false;
        escPressed = sPressed = pPressed = mPressed = false;
    }

    // ==================== ОТРИСОВКА ====================
    void Game::DrawGame(sf::RenderWindow& window, float deltaTime)
    {
        // Сетка
        sf::RectangleShape cell({ GRID_CELL_SIZE - 1.f, GRID_CELL_SIZE - 1.f });
        cell.setFillColor(sf::Color(50, 50, 50));
        for (int x = 0; x < grid.GetWidth(); ++x)
            for (int y = 0; y < grid.GetHeight(); ++y) {
                cell.setPosition(x * GRID_CELL_SIZE, y * GRID_CELL_SIZE);
                window.draw(cell);
            }

        // Камни
        for (const auto& rock : rocks) rock.Draw(window);

        // Яблоки
        auto drawApple = [&](const sf::Texture& tex, const GridPosition& pos, sf::Color tint = sf::Color::White) {
            sf::Sprite sprite(tex);
            float scale = GRID_CELL_SIZE / static_cast<float>(tex.getSize().x);
            sprite.setScale(scale, scale);
            sprite.setOrigin(tex.getSize().x / 2.f, tex.getSize().y / 2.f);
            sprite.setPosition(pos.x * GRID_CELL_SIZE + GRID_CELL_SIZE / 2.f, pos.y * GRID_CELL_SIZE + GRID_CELL_SIZE / 2.f);
            sprite.setColor(tint);
            window.draw(sprite);
            };
        drawApple(appleTexture, apple.GetPosition());
        if (magicAppleActive) drawApple(magicAppleTexture, magicApplePos);
        if (poisonAppleActive) drawApple(appleTexture, poisonApplePos, sf::Color(179, 255, 30));

        // Змейка
        const auto& segments = snake.GetSegments();
        for (size_t i = 0; i < segments.size(); ++i) {
            sf::Texture* texture = snake.GetSegmentTexture(i);
            if (!texture) continue;

            sf::Sprite segmentSprite(*texture);
            float scale = GRID_CELL_SIZE / static_cast<float>(texture->getSize().x);
            segmentSprite.setScale(scale, scale);
            segmentSprite.setOrigin(texture->getSize().x / 2.f, texture->getSize().y / 2.f);
            segmentSprite.setPosition(segments[i].x * GRID_CELL_SIZE + GRID_CELL_SIZE / 2.f,
                segments[i].y * GRID_CELL_SIZE + GRID_CELL_SIZE / 2.f);

            float rotation = 0.f;
            if (i == 0) {
                switch (snake.GetDirection()) {
                case SnakeDirection::Up: rotation = 0.f; break;
                case SnakeDirection::Right: rotation = 90.f; break;
                case SnakeDirection::Down: rotation = 180.f; break;
                case SnakeDirection::Left: rotation = 270.f; break;
                }
            }
            else if (i == segments.size() - 1) {
                const auto& prev = segments[i - 1];
                const auto& curr = segments[i];
                if (prev.x < curr.x) rotation = 270.f;
                else if (prev.x > curr.x) rotation = 90.f;
                else if (prev.y < curr.y) rotation = 0.f;
                else if (prev.y > curr.y) rotation = 180.f;
            }
            else {
                const auto& prev = segments[i - 1];
                const auto& curr = segments[i];
                const auto& next = segments[i + 1];
                if (prev.x == curr.x && curr.x == next.x) rotation = 0.f;
                else if (prev.y == curr.y && curr.y == next.y) rotation = 90.f;
                else {
                    int inX = curr.x - prev.x, inY = curr.y - prev.y;
                    int outX = next.x - curr.x, outY = next.y - curr.y;
                    if (inX == 0 && inY == -1 && outX == 1 && outY == 0) rotation = 180.f;
                    else if (inX == 1 && inY == 0 && outX == 0 && outY == 1) rotation = 270.f;
                    else if (inX == 0 && inY == 1 && outX == -1 && outY == 0) rotation = 0.f;
                    else if (inX == -1 && inY == 0 && outX == 0 && outY == -1) rotation = 90.f;
                    else if (inX == 0 && inY == -1 && outX == -1 && outY == 0) rotation = 270.f;
                    else if (inX == -1 && inY == 0 && outX == 0 && outY == 1) rotation = 180.f;
                    else if (inX == 0 && inY == 1 && outX == 1 && outY == 0) rotation = 90.f;
                    else if (inX == 1 && inY == 0 && outX == 0 && outY == -1) rotation = 0.f;
                }
            }
            segmentSprite.setRotation(rotation);
            window.draw(segmentSprite);
        }

        // Портал
        if (GetDifficultySettings(m_currentDifficulty).hasPortals) {
            float cellSize = GRID_CELL_SIZE;
            float baseRad = cellSize / 2.f - 5.f;
            if (portalGlowScale > portalGlowTargetScale)
                portalGlowScale = std::max(portalGlowTargetScale, portalGlowScale - portalGlowSpeed * deltaTime);

            sf::CircleShape glow((baseRad + 12.f) * portalGlowScale);
            glow.setOrigin(glow.getRadius(), glow.getRadius());
            glow.setFillColor(sf::Color(255, 255, 255, 230));

            sf::CircleShape portal(baseRad);
            portal.setOrigin(baseRad, baseRad);
            portal.setFillColor(sf::Color(0, 210, 255));
            portal.setOutlineThickness(4.f);
            portal.setOutlineColor(sf::Color::White);

            auto drawPortal = [&](const GridPosition& p) {
                sf::Vector2f pos(p.x * cellSize + cellSize / 2.f, p.y * cellSize + cellSize / 2.f);
                glow.setPosition(pos); window.draw(glow);
                portal.setPosition(pos); window.draw(portal);
                };
            drawPortal(portalPos1);
            drawPortal(portalPos2);
        }

        window.draw(scoreText);
        window.draw(menuHintText);
    }

    void Game::Draw(sf::RenderWindow& window)
    {
        window.clear(sf::Color::Black);
        auto drawOverlay = [&](int alpha) {
            sf::RectangleShape overlay({ SCREEN_WIDTH, SCREEN_HEIGHT });
            overlay.setFillColor(sf::Color(0, 0, 0, alpha));
            window.draw(overlay);
            };

        switch (state) {
        case GameState::MAIN_MENU: menuManager.DrawMainMenu(window); break;
        case GameState::DIFFICULTY_SELECT: menuManager.DrawDifficultyMenu(window, m_currentDifficulty); break;
        case GameState::LEADERBOARD: menuManager.DrawLeaderboard(window); break;
        case GameState::MENU:
            DrawGame(window, 0.f);
            menuManager.DrawGameMenu(window);
            break;
        case GameState::SOUND_POPUP:
            DrawGame(window, 0.f);
            soundPopup->Draw(window);
            break;
        case GameState::PLAYING:
        case GameState::PAUSED:
            DrawGame(window, 1.f / 60.f);
            if (state == GameState::PAUSED) {
                drawOverlay(150);
                sf::Text pauseText;
                pauseText.setFont(font);
                pauseText.setString("PAUSED\nPress P to continue");
                pauseText.setCharacterSize(48);
                pauseText.setFillColor(sf::Color::Yellow);
                pauseText.setOrigin(pauseText.getLocalBounds().width / 2, pauseText.getLocalBounds().height / 2);
                pauseText.setPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f);
                window.draw(pauseText);
            }
            break;
        case GameState::GAME_OVER:
            DrawGame(window, 0.f);
            drawOverlay(200);
            window.draw(gameOverText);
            window.draw(finalScoreText);
            static bool spacePressed = false;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                if (!spacePressed) { spacePressed = true; state = GameState::MAIN_MENU; }
            }
            else spacePressed = false;
            break;
        }
    }
}