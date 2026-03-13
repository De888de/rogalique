#include "Snake.h"

namespace SnakeGame
{
    Snake::Snake(const GridPosition& startPos, int initialLength)
        : currentDirection(SnakeDirection::Right)
        , nextDirection(SnakeDirection::Right)
    {
        segments.clear();

        for (int i = 0; i < initialLength; ++i)
        {
            GridPosition pos(startPos.x - i, startPos.y);
            segments.push_back(pos);
        }
    }

    void Snake::Reset(const GridPosition& startPos, int initialLength)
    {
        segments.clear();
        currentDirection = SnakeDirection::Right;
        nextDirection = SnakeDirection::Right;

        for (int i = 0; i < initialLength; ++i)
        {
            GridPosition pos(startPos.x - i, startPos.y);
            segments.push_back(pos);
        }
    }

    void Snake::LoadTextures(const std::string& resourcesPath)
    {
        // Загрузка головы
        std::string headPath = resourcesPath + "Head.png";
        if (!headTexture.loadFromFile(headPath))
        {
            sf::Image img;
            img.create(GRID_CELL_SIZE, GRID_CELL_SIZE, sf::Color::Red);
            headTexture.loadFromImage(img);
        }

        // Загрузка тела (прямое)
        std::string bodyPath = resourcesPath + "Body.png";
        if (!bodyTexture.loadFromFile(bodyPath))
        {
            sf::Image img;
            img.create(GRID_CELL_SIZE, GRID_CELL_SIZE, sf::Color::Green);
            bodyTexture.loadFromImage(img);
        }

        // Загрузка изгиба тела
        std::string bodyBendPath = resourcesPath + "BodyBend.png";
        if (!bodyBendTexture.loadFromFile(bodyBendPath))
        {
            sf::Image img;
            img.create(GRID_CELL_SIZE, GRID_CELL_SIZE, sf::Color::Yellow);
            bodyBendTexture.loadFromImage(img);
        }

        // Загрузка хвоста
        std::string tailPath = resourcesPath + "Tail.png";
        if (!tailTexture.loadFromFile(tailPath))
        {
            sf::Image img;
            img.create(GRID_CELL_SIZE, GRID_CELL_SIZE, sf::Color::Blue);
            tailTexture.loadFromImage(img);
        }
    }

    void Snake::SetDirection(SnakeDirection dir)
    {
        if ((currentDirection == SnakeDirection::Up && dir == SnakeDirection::Down) ||
            (currentDirection == SnakeDirection::Down && dir == SnakeDirection::Up) ||
            (currentDirection == SnakeDirection::Left && dir == SnakeDirection::Right) ||
            (currentDirection == SnakeDirection::Right && dir == SnakeDirection::Left))
        {
            return;
        }

        nextDirection = dir;
    }

    void Snake::Move()
    {
        GridPosition newHead = segments.front();

        switch (nextDirection)
        {
        case SnakeDirection::Up:    newHead.y--; break;
        case SnakeDirection::Down:  newHead.y++; break;
        case SnakeDirection::Left:  newHead.x--; break;
        case SnakeDirection::Right: newHead.x++; break;
        }

        segments.push_front(newHead);
        segments.pop_back();

        currentDirection = nextDirection;
    }

    void Snake::Grow()
    {
        if (!segments.empty())
        {
            GridPosition newTail = segments.back();
            segments.push_back(newTail);
        }
    }

    bool Snake::CheckSelfCollision() const
    {
        if (segments.size() < 2) return false;

        const auto& head = segments.front();

        for (size_t i = 1; i < segments.size(); ++i)
        {
            if (segments[i] == head)
            {
                return true;
            }
        }

        return false;
    }

    bool Snake::CheckPositionCollision(const GridPosition& pos) const
    {
        for (const auto& segment : segments)
        {
            if (segment == pos)
            {
                return true;
            }
        }

        return false;
    }

    SnakeDirection Snake::GetSegmentDirection(size_t index) const
    {
        if (index >= segments.size()) return SnakeDirection::Right;

        if (index == 0)
        {
            return currentDirection;
        }
        else if (index == segments.size() - 1)
        {
            const auto& prev = segments[index - 1];
            const auto& curr = segments[index];

            if (prev.x > curr.x) return SnakeDirection::Right;
            if (prev.x < curr.x) return SnakeDirection::Left;
            if (prev.y > curr.y) return SnakeDirection::Down;
            if (prev.y < curr.y) return SnakeDirection::Up;
        }
        else
        {
            const auto& prev = segments[index - 1];
            const auto& curr = segments[index];
            const auto& next = segments[index + 1];

            if (prev.x == curr.x && curr.x == next.x) return SnakeDirection::Up;
            if (prev.y == curr.y && curr.y == next.y) return SnakeDirection::Right;
        }

        return SnakeDirection::Right;
    }

    SegmentType Snake::GetSegmentType(size_t index) const
    {
        if (index == 0) return SegmentType::Head;
        if (index == segments.size() - 1) return SegmentType::Tail;
        return SegmentType::Body;
    }

    float Snake::GetSegmentRotation(size_t index) const
    {
        if (index >= segments.size()) return 0.0f;

        SegmentType type = GetSegmentType(index);
        SnakeDirection dir = GetSegmentDirection(index);

        if (type == SegmentType::Head)
        {
            switch (dir)
            {
            case SnakeDirection::Up:    return 0.0f;
            case SnakeDirection::Right: return 90.0f;
            case SnakeDirection::Down:  return 180.0f;
            case SnakeDirection::Left:  return 270.0f;
            }
        }
        else if (type == SegmentType::Tail)
        {
            switch (dir)
            {
            case SnakeDirection::Up:    return 180.0f;
            case SnakeDirection::Right: return 270.0f;
            case SnakeDirection::Down:  return 0.0f;
            case SnakeDirection::Left:  return 90.0f;
            }
        }
        else if (type == SegmentType::Body)
        {
            if (index > 0 && index < segments.size() - 1)
            {
                const auto& prev = segments[index - 1];
                const auto& curr = segments[index];
                const auto& next = segments[index + 1];

                // Прямые сегменты
                if (prev.x == curr.x && curr.x == next.x) // Вертикальный
                {
                    return 0.0f;
                }
                else if (prev.y == curr.y && curr.y == next.y) // Горизонтальный
                {
                    return 90.0f;
                }
                else // Угловые сегменты
                {
                    int inX = curr.x - prev.x;
                    int inY = curr.y - prev.y;
                    int outX = next.x - curr.x;
                    int outY = next.y - curr.y;

                    if (inX == 0 && inY == -1 && outX == 1 && outY == 0) return 180.0f;
                    if (inX == 1 && inY == 0 && outX == 0 && outY == 1) return 270.0f;
                    if (inX == 0 && inY == 1 && outX == -1 && outY == 0) return 0.0f;
                    if (inX == -1 && inY == 0 && outX == 0 && outY == -1) return 90.0f;
                    if (inX == 0 && inY == -1 && outX == -1 && outY == 0) return 270.0f;
                    if (inX == -1 && inY == 0 && outX == 0 && outY == 1) return 180.0f;
                    if (inX == 0 && inY == 1 && outX == 1 && outY == 0) return 90.0f;
                    if (inX == 1 && inY == 0 && outX == 0 && outY == -1) return 0.0f;
                }
            }
        }

        return 0.0f;
    }

    sf::Texture* Snake::GetSegmentTexture(size_t index) const
    {
        if (index >= segments.size())
        {
            return nullptr;
        }

        SegmentType type = GetSegmentType(index);

        switch (type)
        {
        case SegmentType::Head:
            return const_cast<sf::Texture*>(&headTexture);

        case SegmentType::Tail:
            return const_cast<sf::Texture*>(&tailTexture);

        case SegmentType::Body:
            if (index > 0 && index < segments.size() - 1)
            {
                const auto& prev = segments[index - 1];
                const auto& curr = segments[index];
                const auto& next = segments[index + 1];

                // Проверяем, является ли сегмент прямым
                if ((prev.x == curr.x && curr.x == next.x) || // Вертикальный
                    (prev.y == curr.y && curr.y == next.y))   // Горизонтальный
                {
                    return const_cast<sf::Texture*>(&bodyTexture);
                }
                else
                {
                    // Угловой сегмент
                    return const_cast<sf::Texture*>(&bodyBendTexture);
                }
            }
            break;
        }

        return const_cast<sf::Texture*>(&bodyTexture);
    }
    void Snake::ResetAfterTeleport(const GridPosition& newHeadPos, SnakeDirection direction, int desiredLength)
    {
        segments.clear();

        // Добавляем новую голову
        segments.push_front(newHeadPos);

        GridPosition current = newHeadPos;

        // Строим тело позади головы в направлении, противоположном движению
        for (int i = 1; i < desiredLength; ++i)
        {
            GridPosition next = current;

            // Двигаемся "назад" относительно направления движения
            switch (direction)
            {
            case SnakeDirection::Up:    next.y += 1; break;
            case SnakeDirection::Down:  next.y -= 1; break;
            case SnakeDirection::Left:  next.x += 1; break;
            case SnakeDirection::Right: next.x -= 1; break;
            }

            // Если вышли за границы — останавливаемся
            if (next.x < 0 || next.x >= 40 || next.y < 0 || next.y >= 30)
                break;

            segments.push_back(next);
            current = next;
        }

        currentDirection = direction;
        nextDirection = direction;
    }
}