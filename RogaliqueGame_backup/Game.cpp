#include "Game.h"
#include "GameState.h"
#include "Application.h"

#include "SaveManager.h"
#include <iostream>
#include <fstream>
#include <algorithm>

namespace RoqaliqueGame
{
    extern Application* g_Application;
    extern Game* g_Game;

    Game::Game()
    {
        std::cout << "[Game] Constructor called (Facade pattern)" << std::endl;

        saveManager = std::make_unique<SaveManager>();

        hasSavedProgress = saveManager->LoadGameProgress(savedLives, savedScore, savedLevel);
        std::cout << "[Game] Saved progress: hasSavedProgress=" << hasSavedProgress
            << ", lives=" << savedLives << ", score=" << savedScore << ", level=" << savedLevel << std::endl;

        options[GameOptions::Sound] = true;
        options[GameOptions::Music] = true;
        options[GameOptions::Vibration] = false;
        options[GameOptions::ShowFPS] = false;

        stateChangeFlag = 0;
        currentScore = 0;
        currentLevel = 0;
    }

    Game::~Game()
    {
        std::cout << "[Game] Destructor called" << std::endl;
        Shutdown();
    }

    void Game::StartGame()
    {
        std::cout << "[Game] StartGame called!" << std::endl;
        isPaused = false;
        currentLevel = 0;
        currentScore = 0;

        if (saveManager)
        {
            saveManager->ClearProgress();
            hasSavedProgress = false;
            savedLives = 3;
            savedScore = 0;
            savedLevel = 0;
        }

        stateStack.clear();
        stateChangeFlag = 0;
        pendingGameStateType = GameStateType::None;

        SwitchStateTo(GameStateType::Playing);
        std::cout << "[Game] StartGame finished, pending state set" << std::endl;
    }

    void Game::ContinueGame()
    {
        if (!saveManager->LoadGameProgress(savedLives, savedScore, savedLevel))
        {
            StartGame();
            return;
        }
        currentLevel = savedLevel;
        currentScore = savedScore;
        stateStack.clear();
        SwitchStateTo(GameStateType::Playing);
    }

      

    bool Game::HasSavedProgress() const
    {
        return hasSavedProgress;
    }

    void Game::SaveCurrentRecord()
    {
        if (saveManager)
            saveManager->SaveRecord("Player", currentScore);
    }

    void Game::SaveCurrentProgress(int lives, int score, int level)
    {
        if (saveManager)
        {
            saveManager->SaveGameProgress(lives, score, level);
            hasSavedProgress = true;
            savedLives = lives;
            savedScore = score;
            savedLevel = level;
            currentScore = score;
            currentLevel = level;
            std::cout << "[Game] Progress saved: lives=" << lives
                << ", score=" << score << ", level=" << level << std::endl;
        }
    }

    void Game::PauseGame() { if (!stateStack.empty()) isPaused = true; }
    void Game::ResumeGame() { if (isPaused) isPaused = false; }

    void Game::WinGame(int finalScore)
    {
        std::cout << "[Game] WinGame called with score: " << finalScore << std::endl;
        currentScore = finalScore;
        SaveCurrentRecord();

        if (saveManager)
            saveManager->ClearProgress();
        hasSavedProgress = false;

        SwitchStateTo(GameStateType::Win, finalScore);
    }

    void Game::LoseGame(int finalScore)
    {
        std::cout << "[Game] LoseGame called with score: " << finalScore << std::endl;
        currentScore = finalScore;
        SaveCurrentRecord();

        if (saveManager)
            saveManager->ClearProgress();
        hasSavedProgress = false;

        SwitchStateTo(GameStateType::GameOver, finalScore);
    }

    void Game::ReturnToMenu()
    {
        std::cout << "[Game] ReturnToMenu called!" << std::endl;

        if (!stateStack.empty() && stateStack.back()->GetType() == GameStateType::Playing)
        {
            SaveCurrentProgress(savedLives, currentScore, currentLevel);
            std::cout << "[Game] Progress saved before returning to menu" << std::endl;
        }

        stateStack.clear();
        stateChangeFlag = 0;
        pendingGameStateType = GameStateType::None;

        if (g_Application)
            g_Application->ReturnToMenu();
    }

    void Game::ExitGame() { if (g_Application) g_Application->window.close(); }
    void Game::QuitGame() { ExitGame(); }

    void Game::LoadNextLevel() { currentLevel++; }
    void Game::RestartLevel() { SwitchStateTo(GameStateType::Playing); }

    int Game::GetCurrentLevel() const { return currentLevel; }
    int Game::GetTotalLevels() const
    {
        LevelLoader loader;
        return loader.GetLevelCount();
    }

    void Game::ShowRecords()
    {
        std::cout << "=== TOP RECORDS ===" << std::endl;
        for (const auto& p : saveManager->GetTopRecords(10))
            std::cout << p.first << ": " << p.second << std::endl;
    }

    const Game::RecordsTable& Game::GetRecordsTable() const
    {
        static RecordsTable empty;
        return empty;
    }

    int Game::GetRecordByPlayerId(const std::string& playerId) const
    {
        return saveManager->GetBestScore(playerId);
    }

    void Game::UpdateRecord(const std::string& playerId, int score)
    {
        saveManager->SaveRecord(playerId, score);
    }

    void Game::AddToLeaderboard(const std::string& name, int score)
    {
        if (saveManager)
            saveManager->AddToLeaderboard(name, score);
    }

    std::string Game::GetLeaderboardText() const
    {
        if (saveManager)
            return saveManager->GetLeaderboardText();
        return "Leaderboard not available";
    }

    bool Game::IsOptionEnabled(GameOptions option) const
    {
        auto it = options.find(option);
        return it != options.end() ? it->second : false;
    }

    void Game::SetOption(GameOptions option, bool value)
    {
        options[option] = value;
    }

    void Game::PushState(GameStateType stateType, bool isExclusivelyVisible)
    {
        (void)stateType;
        (void)isExclusivelyVisible;
    }

    void Game::PopState()
    {
        if (!stateStack.empty())
            stateStack.pop_back();
    }

    void Game::SwitchStateTo(GameStateType newState, int score)
    {
        pendingGameStateType = newState;
        pendingScore = score;
        stateChangeFlag = 1;
        std::cout << "[Game] SwitchStateTo: " << (int)newState << std::endl;
    }

    void Game::HandleWindowEvent(const sf::Event& event)
    {
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
        {
            ReturnToMenu();
            return;
        }

        if (!stateStack.empty())
            stateStack.back()->HandleWindowEvent(event);
    }

    bool Game::Update(float timeDelta)
    {
        if (isPaused) return true;

        if (stateChangeFlag == 1)
            stateStack.clear();

        if (pendingGameStateType != GameStateType::None)
        {
            std::cout << "[Game::Update] Creating state: " << (int)pendingGameStateType << std::endl;

            if (pendingGameStateType == GameStateType::Playing)
            {
               

                stateStack.push_back(std::make_unique<GameStatePlaying>(
                    currentLevel,     // уровень из сохранения
                    currentScore,     // очки из сохранения
                    savedLives        // жизни из сохранения
                ));
            }
            else if (pendingGameStateType == GameStateType::GameOver)
                stateStack.push_back(std::make_unique<GameStateGameOver>(pendingScore));
            else if (pendingGameStateType == GameStateType::Win)
                stateStack.push_back(std::make_unique<GameStateWin>(pendingScore));

            pendingGameStateType = GameStateType::None;
        }

        stateChangeFlag = 0;

        if (!stateStack.empty())
        {
            stateStack.back()->Update(timeDelta);
            return true;
        }
        return false;
    }

    void Game::Draw(sf::RenderWindow& window)
    {
        if (!stateStack.empty())
            stateStack.back()->Draw(window);
    }

    void Game::UpdateGame(float timeDelta, sf::RenderWindow& window)
    {
        HandleWindowEvents(window);
        window.clear(sf::Color::Black);
        Update(timeDelta);
        Draw(window);
        window.display();
    }

    void Game::HandleWindowEvents(sf::RenderWindow& window)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            HandleWindowEvent(event);
        }
    }

    void Game::Shutdown()
    {
        stateStack.clear();
    }

} // namespace ArkanoidGame