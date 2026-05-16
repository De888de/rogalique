#pragma once
#include <functional>

namespace SnakeGame
{
    struct GridPosition
    {
        int x = 0;
        int y = 0;

        // Добавляем конструктор по умолчанию
        GridPosition() = default;

        // Добавляем конструктор с параметрами
        GridPosition(int xPos, int yPos) : x(xPos), y(yPos) {}

        bool operator==(const GridPosition& other) const
        {
            return x == other.x && y == other.y;
        }

        bool operator!=(const GridPosition& other) const
        {
            return !(*this == other);
        }
    };

    struct GridPositionHash
    {
        std::size_t operator()(const GridPosition& pos) const
        {
            return std::hash<int>()(pos.x) ^ (std::hash<int>()(pos.y) << 1);
        }
    };

    enum class Direction
    {
        Up,
        Down,
        Left,
        Right
    };

    class Grid
    {
    private:
        int width;
        int height;

    public:
        Grid(int w, int h);

        int GetWidth() const { return width; }
        int GetHeight() const { return height; }

        bool IsValidPosition(const GridPosition& pos) const;
        GridPosition GetRandomPosition() const;
    };
}