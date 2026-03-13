#pragma once
#include <deque>
#include <SFML/Graphics.hpp>
#include "Grid.h"
#include "Constants.h"

namespace SnakeGame
{
    enum class SnakeDirection
    {
        Up,
        Down,
        Left,
        Right
    };

    inline bool operator==(SnakeDirection a, SnakeDirection b)
    {
        return static_cast<int>(a) == static_cast<int>(b);
    }

    inline bool operator!=(SnakeDirection a, SnakeDirection b)
    {
        return !(a == b);
    }

    enum class SegmentType
    {
        Head,
        Body,
        Tail
    };

    class Snake
    {
    private:
        std::deque<GridPosition> segments;
        SnakeDirection currentDirection;
        SnakeDirection nextDirection;

        // Только 4 текстуры, как у тебя
        sf::Texture headTexture;
        sf::Texture bodyTexture;
        sf::Texture bodyBendTexture;
        sf::Texture tailTexture;

    public:
        Snake() = default;
        Snake(const GridPosition& startPos, int initialLength);

        void LoadTextures(const std::string& resourcesPath);
        void Reset(const GridPosition& startPos, int initialLength);
        void SetDirection(SnakeDirection dir);
        SnakeDirection GetDirection() const { return currentDirection; }

        void Move();
        void Grow();
        void TeleportHead(const GridPosition& newPos) { segments.front() = newPos; }
        // Специальный метод для корректной телепортации
        void ResetAfterTeleport(const GridPosition& newHeadPos, SnakeDirection direction, int desiredLength);

        const GridPosition& GetHead() const { return segments.front(); }
        const std::deque<GridPosition>& GetSegments() const { return segments; }
        int GetLength() const { return static_cast<int>(segments.size()); }

        bool CheckSelfCollision() const;
        bool CheckPositionCollision(const GridPosition& pos) const;
        bool IsOccupied(const GridPosition& pos) const { return CheckPositionCollision(pos); }

        sf::Texture* GetSegmentTexture(size_t index) const;

    private:
        SnakeDirection GetSegmentDirection(size_t index) const;
        SegmentType GetSegmentType(size_t index) const;
        float GetSegmentRotation(size_t index) const;  // НОВЫЙ МЕТОД для угла поворота
    };
}