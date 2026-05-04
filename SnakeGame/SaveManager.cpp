#include "SaveManager.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <filesystem>
#include <sys/stat.h>

#ifdef _WIN32
#include <direct.h>
#define mkdir _mkdir
#else
#include <sys/stat.h>
#endif

namespace ArkanoidGame
{
    SaveManager::SaveManager()
        : recordsFile("Resources/saves/records.txt")
        , progressFile("Resources/saves/progress.txt")
    {
        EnsureDirectoriesExist();
        LoadRecords();
    }

    SaveManager::~SaveManager()
    {
        SaveRecordsToFile();
    }
    void SaveManager::EnsureDirectoriesExist()
    {
#ifdef _WIN32
        // Подавляем warning C6031
#pragma warning(push)
#pragma warning(disable: 6031)
        _mkdir("Resources");
        _mkdir("Resources\\saves");
#pragma warning(pop)
#else
        mkdir("Resources", 0777);
        mkdir("Resources/saves", 0777);
#endif
    }

    void SaveManager::LoadRecords()
    {
        std::ifstream file(recordsFile);
        if (file.is_open())
        {
            std::string name;
            int score;
            while (file >> name >> score)
            {
                records[name] = score;
                leaderboard.AddScore(name, score);
            }
            file.close();
            std::cout << "[SaveManager] Loaded " << records.size() << " records" << std::endl;
        }
        else
        {
            std::cout << "[SaveManager] No existing records file, starting fresh" << std::endl;
        }
    }

    void SaveManager::SaveRecordsToFile()
    {
        std::ofstream file(recordsFile);
        if (file.is_open())
        {
            for (const auto& [name, score] : records)
            {
                file << name << " " << score << std::endl;
            }
            file.close();
            std::cout << "[SaveManager] Saved " << records.size() << " records" << std::endl;
        }
        else
        {
            std::cout << "[SaveManager] ERROR: Could not save records to " << recordsFile << std::endl;
        }
    }

    void SaveManager::SaveRecord(const std::string& playerName, int score)
    {
        auto it = records.find(playerName);
        if (it == records.end() || score > it->second)
        {
            records[playerName] = score;
            SaveRecordsToFile();
            std::cout << "[SaveManager] New record for " << playerName << ": " << score << std::endl;
        }

        // Также добавляем в лидерборд
        leaderboard.UpdatePlayerScore(score);
    }

    int SaveManager::GetBestScore(const std::string& playerName) const
    {
        auto it = records.find(playerName);
        return it != records.end() ? it->second : 0;
    }

    std::vector<std::pair<std::string, int>> SaveManager::GetTopRecords(int count) const
    {
        auto topEntries = leaderboard.GetTopEntries(count);
        std::vector<std::pair<std::string, int>> result;
        for (const auto& entry : topEntries)
        {
            result.emplace_back(entry.name, entry.score);
        }
        return result;
    }

    void SaveManager::AddToLeaderboard(const std::string& name, int score)
    {
        leaderboard.AddScore(name, score);
    }

    void SaveManager::UpdatePlayerInLeaderboard(int score)
    {
        leaderboard.UpdatePlayerScore(score);
    }

    std::string SaveManager::GetLeaderboardText() const
    {
        return leaderboard.Format();
    }

    void SaveManager::SaveGameProgress(int lives, int score, int level)
    {
        std::cout << "[SaveManager] SaveGameProgress called: lives=" << lives
            << ", score=" << score << ", level=" << level << std::endl;

        std::ofstream file(progressFile);
        if (file.is_open())
        {
            file << lives << " " << score << " " << level << std::endl;
            file.close();
            std::cout << "[SaveManager] Progress saved successfully to: " << progressFile << std::endl;
        }
        else
        {
            std::cout << "[SaveManager] ERROR: Could not open progress file: " << progressFile << std::endl;
        }
    }

    bool SaveManager::LoadGameProgress(int& outLives, int& outScore, int& outLevel)
    {
        std::ifstream file(progressFile);
        if (file.is_open())
        {
            file >> outLives >> outScore >> outLevel;
            file.close();
            std::cout << "[SaveManager] Loaded progress: lives=" << outLives
                << ", score=" << outScore << ", level=" << outLevel << std::endl;
            return true;
        }
        std::cout << "[SaveManager] No saved progress file found" << std::endl;
        return false;
    }

    void SaveManager::ClearProgress()
    {
        std::remove(progressFile.c_str());
        std::cout << "[SaveManager] Progress cleared" << std::endl;
    }

    bool SaveManager::HasSavedProgress() const
    {
        std::ifstream file(progressFile);
        return file.is_open();
    }

} // namespace ArkanoidGame
