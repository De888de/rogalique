#pragma once
#include <SFML/Graphics.hpp>

namespace rogalique {

    class Bullet {
    public:
        // Конструктор с 2 аргументами (позиция, направление)
        Bullet(sf::Vector2f position, sf::Vector2f direction)
            : m_active(false), m_position(position), m_velocity(direction) {
            std::cout << "[Bullet] Created (2 args) - but shooting is disabled" << std::endl;
        }
        
        // Конструктор с 3 аргументами (позиция, направление, скорость)
        Bullet(sf::Vector2f position, sf::Vector2f direction, float speed)
            : m_active(false), m_position(position), m_velocity(direction * speed) {
            std::cout << "[Bullet] Created (3 args) - but shooting is disabled" << std::endl;
        }

        void Update(float deltaTime) {
            // Заглушка - пули не двигаются
        }

        void Render(sf::RenderWindow& window) {
            // Заглушка - ничего не рисуем
        }

        bool IsActive() const { 
            return false; // Пули никогда не активны
        }
        
    private:
        sf::Vector2f m_position;
        sf::Vector2f m_velocity;
        bool m_active;
    };
    
}
