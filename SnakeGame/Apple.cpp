#include "Apple.h"
#include "Constants.h" 
#include <cstdlib>

namespace SnakeGame
{
    void Apple::Generate(const Grid& grid, const std::function<bool(const GridPosition&)>& isOccupied)
    {
        const int maxAttempts = 1000;

        for (int attempt = 0; attempt < maxAttempts; ++attempt)
        {
            GridPosition candidate = grid.GetRandomPosition();

            if (!isOccupied(candidate))
            {
                position = candidate;
                return;
            }
        }

        // Если не нашли свободное место, пробуем пройтись по всем клеткам
        for (int x = 0; x < grid.GetWidth(); ++x)
        {
            for (int y = 0; y < grid.GetHeight(); ++y)
            {
                GridPosition candidate(x, y);  // Используем новый конструктор
                if (!isOccupied(candidate))
                {
                    position = candidate;
                    return;
                }
            }
        }

        // Если вообще нет свободных мест - победа!
        // TODO: обработать случай заполнения всей карты
        position = GridPosition(0, 0);  // Заглушка
    }
}