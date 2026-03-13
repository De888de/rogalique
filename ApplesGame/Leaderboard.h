#pragma once
#include <map>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

namespace ApplesGame
{
    struct LeaderboardEntry
    {
        std::string name;
        int score = 0;
    };

    class Leaderboard
    {
    private:
        // std::map automatically sorts by key (points)
        // The reverse iterator gives us a descending sort
        std::map<int, std::vector<LeaderboardEntry>> entriesByScore;
        std::string playerName = "Player";

    public:
        void Generate();
        void UpdatePlayerScore(int score);
        std::vector<LeaderboardEntry> GetTopEntries(int count = 5) const;
        std::string Format() const;
        void Clear();
    };
}