#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

namespace RoqaliqueGame
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

        // ========== Прогресс игры ==========
        void SaveGameProgress(int lives, int score, int level);
        bool LoadGameProgress(int& outLives, int& outScore, int& outLevel);
        void ClearProgress();
        bool HasSavedProgress() const;

    private:
        std::unordered_map<std::string, int> records;

        // Пути к файлам сохранений
        std::string recordsFile;
        std::string progressFile;

        void LoadRecords();
        void SaveRecordsToFile();
        void EnsureDirectoriesExist();
    };

} // namespace ArkanoidGame