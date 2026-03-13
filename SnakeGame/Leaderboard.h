#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

namespace SnakeGame
{
    struct LeaderboardEntry
    {
        std::string playerName;
        int score = 0;
        bool operator<(const LeaderboardEntry& other) const { return score < other.score; }
        bool operator>(const LeaderboardEntry& other) const { return score > other.score; }
    };

    class Leaderboard
    {
    private:
        std::vector<LeaderboardEntry> entries;
        std::string filename = "leaderboard.dat";
        const int maxEntries = 10;

    public:
        Leaderboard() = default;

        void Load()
        {
            entries.clear();
            std::ifstream file(filename, std::ios::binary);
            if (!file.is_open())
            {
                GenerateTestData();
                Save();
                return;
            }

            size_t count;
            file.read(reinterpret_cast<char*>(&count), sizeof(count));

            for (size_t i = 0; i < count; ++i)
            {
                LeaderboardEntry entry;
                size_t nameLength;
                file.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
                entry.playerName.resize(nameLength);
                file.read(&entry.playerName[0], nameLength);
                file.read(reinterpret_cast<char*>(&entry.score), sizeof(entry.score));
                entries.push_back(entry);
            }
            file.close();
        }

        void Save() const
        {
            std::ofstream file(filename, std::ios::binary);
            if (!file.is_open()) return;

            size_t count = entries.size();
            file.write(reinterpret_cast<const char*>(&count), sizeof(count));

            for (const auto& entry : entries)
            {
                size_t nameLength = entry.playerName.size();
                file.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
                file.write(entry.playerName.c_str(), nameLength);
                file.write(reinterpret_cast<const char*>(&entry.score), sizeof(entry.score));
            }
            file.close();
        }

        void AddScore(int score, const std::string& playerName = "Player")
        {
            LeaderboardEntry newEntry;
            newEntry.playerName = playerName;
            newEntry.score = score;
            entries.push_back(newEntry);

            for (size_t i = 0; i < entries.size() - 1; ++i)
                for (size_t j = 0; j < entries.size() - i - 1; ++j)
                    if (entries[j].score < entries[j + 1].score)
                        std::swap(entries[j], entries[j + 1]);

            if (entries.size() > maxEntries)
                entries.resize(maxEntries);
        }

        void GenerateTestData()
        {
            entries = {
                {"Alice", 150}, {"Bob", 120}, {"Carol", 95}, {"Dave", 80},
                {"Eve", 65}, {"Frank", 50}, {"Grace", 35}, {"Henry", 25},
                {"Ivy", 15}, {"Jack", 5}
            };
        }

        std::string Format() const
        {
            std::string result = "=== LEADERBOARD ===\n\n";
            for (size_t i = 0; i < entries.size() && i < 5; ++i)
            {
                result += std::to_string(i + 1) + ". " + entries[i].playerName;
                int dotsCount = 20 - static_cast<int>(entries[i].playerName.length()) -
                    static_cast<int>(std::to_string(entries[i].score).length());
                result += std::string(dotsCount > 0 ? dotsCount : 1, '.') + " " +
                    std::to_string(entries[i].score) + "\n";
            }
            if (entries.empty()) result += "No records yet!\n";
            result += "\n=================";
            return result;
        }

        const std::vector<LeaderboardEntry>& GetAllEntries() const { return entries; }
        bool IsEmpty() const { return entries.empty(); }
        void Clear() { entries.clear(); }
    };
}