#pragma once
#include <vector>
#include <algorithm>
#include "Leaderboard.h" 

namespace ApplesGame
{
    
    template<typename T>
    void Sort(std::vector<T>& items, bool (*comparator)(const T& a, const T& b))
    {
        if (items.size() <= 1) return;

        // Bubble sorting in descending order
        for (size_t i = 0; i < items.size() - 1; ++i)
        {
            for (size_t j = 0; j < items.size() - i - 1; ++j)
            {
                if (comparator(items[j], items[j + 1]))
                {
                    std::swap(items[j], items[j + 1]);
                }
            }
        }
    }

    // Comparison function for LeaderboardEntry by descending score
    bool CompareLeaderboardByScoreDesc(const LeaderboardEntry& a, const LeaderboardEntry& b);
}
