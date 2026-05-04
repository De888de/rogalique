#pragma once
#include <map>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

namespace ArkanoidGame
{
    struct LeaderboardEntry
    {
        std::string name;
        int score = 0;
    };

    class Leaderboard
    {
    private:
        std::map<int, std::vector<LeaderboardEntry>> entriesByScore;
        std::string playerName = "Player";
        std::string saveFile = "Resources/saves/leaderboard.txt";

        void LoadFromFile();
        void SaveToFile();

    public:
        Leaderboard();

        void AddScore(const std::string& name, int score);
        void UpdatePlayerScore(int score);
        std::vector<LeaderboardEntry> GetTopEntries(int count = 5) const;
        std::string Format() const;
        void Clear();
        void SetPlayerName(const std::string& name) { playerName = name; }
    };
}
