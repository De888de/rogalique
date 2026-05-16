#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include "Leaderboard.h"

namespace ArkanoidGame
{
    class SaveManager
    {
    public:
        SaveManager();
        ~SaveManager();

        // ========== Рекорды ==========
        void SaveRecord(const std::string& playerName, int score);
        int GetBestScore(const std::string& playerName) const;
        std::vector<std::pair<std::string, int>> GetTopRecords(int count = 10) const;

        // ========== Лидерборд ==========
        Leaderboard& GetLeaderboard() { return leaderboard; }
        void AddToLeaderboard(const std::string& name, int score);
        void UpdatePlayerInLeaderboard(int score);
        std::string GetLeaderboardText() const;

        // ========== Прогресс игры ==========
        void SaveGameProgress(int lives, int score, int level);
        bool LoadGameProgress(int& outLives, int& outScore, int& outLevel);
        void ClearProgress();
        bool HasSavedProgress() const;

    private:
        std::unordered_map<std::string, int> records;
        Leaderboard leaderboard;

        // Пути к файлам сохранений
        std::string recordsFile;
        std::string progressFile;

        void LoadRecords();
        void SaveRecordsToFile();
        void EnsureDirectoriesExist();
    };

} // namespace ArkanoidGame