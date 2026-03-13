#include "Rock.h"
#include "Constants.h"
#include <iostream>
#include <filesystem>

namespace SnakeGame
{
    // Локальные статические переменные (ленивая инициализация)
    sf::Texture& Rock::GetTexture()
    {
        static sf::Texture texture;
        return texture;
    }

    bool& Rock::GetTextureLoaded()
    {
        static bool loaded = false;
        return loaded;
    }

    bool Rock::LoadTexture(const std::string& path)
    {
        bool& loaded = GetTextureLoaded();
        if (loaded) return true;

        std::cout << "[Rock] Trying to load texture from: " << path << std::endl;

        sf::Texture& tex = GetTexture();
        loaded = tex.loadFromFile(path);

        if (!loaded)
        {
            std::cerr << "[Rock] ERROR: Failed to load " << path << std::endl;

            if (std::filesystem::exists(path))
                std::cerr << "[Rock] File exists but SFML failed to load it." << std::endl;
            else
                std::cerr << "[Rock] File does NOT exist!" << std::endl;

            // Fallback
            sf::Image img;
            img.create(GRID_CELL_SIZE, GRID_CELL_SIZE, sf::Color(110, 110, 120));
            tex.loadFromImage(img);
            loaded = true;

            std::cout << "[Rock] Using fallback gray texture." << std::endl;
        }
        else
        {
            auto size = tex.getSize();
            std::cout << "[Rock] SUCCESS! Texture size: " << size.x << "x" << size.y << std::endl;
        }
        return loaded;
    }

    Rock::Rock(const GridPosition& pos)
        : m_position(pos)
        , m_sprite()
    {
        sf::Texture& tex = GetTexture();
        bool& loaded = GetTextureLoaded();

        if (loaded && tex.getSize().x > 0)
        {
            m_sprite.setTexture(tex, true);

            float scaleX = static_cast<float>(GRID_CELL_SIZE) / tex.getSize().x;
            float scaleY = static_cast<float>(GRID_CELL_SIZE) / tex.getSize().y;

            m_sprite.setScale(scaleX, scaleY);
            m_sprite.setPosition(
                static_cast<float>(pos.x * GRID_CELL_SIZE),
                static_cast<float>(pos.y * GRID_CELL_SIZE)
            );
        }
    }

    void Rock::Draw(sf::RenderWindow& window) const
    {
        if (GetTextureLoaded() && GetTexture().getSize().x > 0)
        {
            window.draw(m_sprite);
        }
    }
}