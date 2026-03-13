#pragma once
#include <SFML/Graphics.hpp>
#include "Grid.h"
#include "Constants.h"

namespace SnakeGame
{
    class Rock
    {
    private:
        GridPosition m_position{};
        sf::Sprite m_sprite;

        // Убираем статические члены из заголовка
        static sf::Texture& GetTexture();           // ← новый способ
        static bool& GetTextureLoaded();            // ← новый способ

    public:
        Rock() = delete;
        explicit Rock(const GridPosition& pos);

        static bool LoadTexture(const std::string& path);

        const GridPosition& GetPosition() const { return m_position; }
        void Draw(sf::RenderWindow& window) const;
    };
}