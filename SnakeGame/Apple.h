#pragma once
#include "Grid.h"
#include <functional>

namespace SnakeGame
{
    class Apple
    {
    private:
        GridPosition position;

    public:
        Apple() = default;  // Добавляем конструктор по умолчанию

        void Generate(const Grid& grid, const std::function<bool(const GridPosition&)>& isOccupied);
        const GridPosition& GetPosition() const { return position; }
        void SetPosition(const GridPosition& pos) { position = pos; }
    };
}