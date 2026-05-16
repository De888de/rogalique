#include "Leaderboard.h"
#include <sstream>
#include <algorithm>
#include <fstream>
#include <iostream>

namespace RoqaliqueGame
{
    Leaderboard::Leaderboard()
    {
        LoadFromFile();
    }

    void Leaderboard::LoadFromFile()
    {
        entriesByScore.clear();

        std::ifstream file(saveFile);
        if (file.is_open())
        {
            std::string name;
            int score;
            while (file >> name >> score)
            {
                entriesByScore[score].push_back({ name, score });
            }
            file.close();
            std::cout << "[Leaderboard] Loaded " << GetTopEntries(100).size() << " records" << std::endl;
        }
        else
        {
            // Добавляем несколько демо-рекордов
            AddScore("Master", 1500);
            AddScore("ProGamer", 1250);
            AddScore("ArkanoidFan", 980);
            AddScore("Noob", 320);
            AddScore("Beginner", 150);
            std::cout << "[Leaderboard] Created demo records" << std::endl;
        }
    }

    void Leaderboard::SaveToFile()
    {
        std::ofstream file(saveFile);
        if (file.is_open())
        {
            for (const auto& [score, entries] : entriesByScore)
            {
                for (const auto& entry : entries)
                {
                    file << entry.name << " " << entry.score << std::endl;
                }
            }
            file.close();
        }
    }

    void Leaderboard::AddScore(const std::string& name, int score)
    {
        entriesByScore[score].push_back({ name, score });
        SaveToFile();
        std::cout << "[Leaderboard] Added record: " << name << " - " << score << std::endl;
    }

    void Leaderboard::UpdatePlayerScore(int score)
    {
        // Удаляем старую запись игрока
        for (auto it = entriesByScore.begin(); it != entriesByScore.end(); )
        {
            auto& entries = it->second;
            auto playerIt = std::find_if(entries.begin(), entries.end(),
                [this](const LeaderboardEntry& e) { return e.name == playerName; });

            if (playerIt != entries.end())
            {
                entries.erase(playerIt);
                if (entries.empty())
                {
                    it = entriesByScore.erase(it);
                }
                else
                {
                    ++it;
                }
                break;
            }
            else
            {
                ++it;
            }
        }

        // Добавляем новую запись
        entriesByScore[score].push_back({ playerName, score });
        SaveToFile();
    }

    std::vector<LeaderboardEntry> Leaderboard::GetTopEntries(int count) const
    {
        std::vector<LeaderboardEntry> result;

        for (auto it = entriesByScore.rbegin(); it != entriesByScore.rend() && result.size() < count; ++it)
        {
            for (const auto& entry : it->second)
            {
                if (result.size() < count)
                {
                    result.push_back(entry);
                }
                else
                {
                    break;
                }
            }
        }

        return result;
    }

    std::string Leaderboard::Format() const
    {
        std::stringstream ss;
        ss << "\n     === TOP RECORDS ===\n\n";

        auto topEntries = GetTopEntries(5);

        int rank = 1;
        for (const auto& entry : topEntries)
        {
            ss << "     " << rank << ". " << entry.name;

            int dotsCount = 25 - static_cast<int>(entry.name.length()) - static_cast<int>(std::to_string(entry.score).length());
            for (int i = 0; i < dotsCount; ++i)
            {
                ss << ".";
            }

            ss << " " << entry.score << "\n";
            rank++;
        }

        ss << "\n     =================\n";
        return ss.str();
    }

    void Leaderboard::Clear()
    {
        entriesByScore.clear();
        SaveToFile();
    }
}