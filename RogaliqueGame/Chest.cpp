#include "Chest.h"
#include <iostream>

namespace rogalique
{
    Chest::Chest()
    {
        m_shape.setSize(sf::Vector2f(32, 32));
        m_shape.setFillColor(sf::Color(255, 215, 0)); // золотой
        m_shape.setOrigin(16, 16);
    }
    
    void Chest::Update(float deltaTime)
    {
        // Пока ничего не делаем
        (void)deltaTime;
    }
    
    void Chest::Render(sf::RenderWindow& window)
    {
        if (!m_isCollected)
        {
            m_shape.setPosition(m_position);
            window.draw(m_shape);
        }
    }
    
    void Chest::Collect()
    {
        if (!m_isCollected)
        {
            m_isCollected = true;
            std::cout << "[Chest] Collected! +10 gold" << std::endl;
        }
    }
}
