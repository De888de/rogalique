#include "Leaderboard.h"
#include <sstream>
#include <algorithm>

namespace ApplesGame
{
    void Leaderboard::Generate()
    {
        entriesByScore.clear();

        std::vector<std::pair<std::string, int>> fakePlayers = {
            {"Alice", 120}, {"Bob", 85}, {"Carol", 55}, {"Dave", 30},
            {"Eve", 150}, {"Frank", 95}, {"Grace", 70}, {"Henry", 45},
            {"Ivy", 110}, {"Jack", 65}
        };

        for (const auto& player : fakePlayers)
        {
            LeaderboardEntry entry;
            entry.name = player.first;
            entry.score = player.second;
            entriesByScore[player.second].push_back(entry);
        }

        LeaderboardEntry playerEntry;
        playerEntry.name = playerName;
        playerEntry.score = 0;
        entriesByScore[0].push_back(playerEntry);
    }

    void Leaderboard::UpdatePlayerScore(int score)
    {
        
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

        // Adding a new entry
        LeaderboardEntry playerEntry;
        playerEntry.name = playerName;
        playerEntry.score = score;
        entriesByScore[score].push_back(playerEntry);
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
            }
        }

        return result;
    }

    std::string Leaderboard::Format() const
    {
        std::stringstream ss;
        ss << "=====================\n\n";

        auto topEntries = GetTopEntries(5);

        int rank = 1;
        for (const auto& entry : topEntries)
        {
            ss << rank << ". " << entry.name;

            int dotsCount = 20 - static_cast<int>(entry.name.length()) - static_cast<int>(std::to_string(entry.score).length());
            for (int i = 0; i < dotsCount; ++i)
            {
                ss << ".";
            }

            ss << " " << entry.score << "\n";
            rank++;
        }

        ss << "\n=====================";
        return ss.str();
    }

    void Leaderboard::Clear()
    {
        entriesByScore.clear();
    }
}