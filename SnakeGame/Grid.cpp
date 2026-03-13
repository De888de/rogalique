#include "Grid.h"
#include <cstdlib>

namespace SnakeGame
{
    Grid::Grid(int w, int h) : width(w), height(h) {}

    bool Grid::IsValidPosition(const GridPosition& pos) const
    {
        return pos.x >= 0 && pos.x < width &&
            pos.y >= 0 && pos.y < height;
    }

    GridPosition Grid::GetRandomPosition() const
    {
        GridPosition pos;
        pos.x = rand() % width;
        pos.y = rand() % height;
        return pos;
    }
}