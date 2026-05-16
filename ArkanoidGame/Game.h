#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <memory>
#include <vector>
#include "GameState.h"
#include "SaveManager.h"

namespace RoqaliqueGame
{
    class Application;
    class LevelLoader;

    enum class GameOptions
    {
        Sound,
        Music,
        Vibration,
        ShowFPS
    };

    class Game
    {
    public:
        Game();
        ~Game();

        // ========== Основные методы фасада ==========
        void StartGame();
        void ContinueGame();
        void PauseGame();
        void ResumeGame();
        void WinGame(int finalScore = 0);
        void LoseGame(int finalScore = 0);
        void ExitGame();
        void QuitGame();
        void ReturnToMenu();

        bool HasSavedProgress() const;
        void SaveCurrentProgress(int lives, int score, int level);

        // ========== Управление уровнями ==========
        void LoadNextLevel();
        void RestartLevel();
        int GetCurrentLevel() const;
        int GetTotalLevels() const;

        // ========== Рекорды и лидерборд ==========
        using RecordsTable = std::unordered_map<std::string, int>;
        void SaveCurrentRecord();
        void ShowRecords();
        const RecordsTable& GetRecordsTable() const;
        int GetRecordByPlayerId(const std::string& playerId) const;
        void UpdateRecord(const std::string& playerId, int score);

        void AddToLeaderboard(const std::string& name, int score);
        std::string GetLeaderboardText() const;

        // ========== Опции игры ==========
        bool IsOptionEnabled(GameOptions option) const;
        void SetOption(GameOptions option, bool value);

        // ========== Методы для Application ==========
        void UpdateGame(float timeDelta, sf::RenderWindow& window);
        void HandleWindowEvent(const sf::Event& event);

    private:
        void HandleWindowEvents(sf::RenderWindow& window);
        bool Update(float timeDelta);
        void Draw(sf::RenderWindow& window);
        void Shutdown();

        void PushState(GameStateType stateType, bool isExclusivelyVisible);
        void PopState();
        void SwitchStateTo(GameStateType newState, int score = 0);

        std::vector<std::unique_ptr<GameState>> stateStack;
        int stateChangeFlag = 0;
        GameStateType pendingGameStateType = GameStateType::None;
        int pendingScore = 0;

        std::unique_ptr<SaveManager> saveManager;

        bool isPaused = false;
        int currentScore = 0;
        int currentLevel = 0;

        int savedLives = 3;
        int savedScore = 0;
        int savedLevel = 0;
        bool hasSavedProgress = false;

        std::unordered_map<GameOptions, bool> options;

        friend class GameStatePlaying;
        friend class GameStateGameOver;
        friend class GameStateWin;
    };

    Game& GetGame();
}