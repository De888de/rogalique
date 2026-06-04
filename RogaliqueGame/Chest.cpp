#include "Chest.h"
#include "SoundManager.h"
#include "Application.h"
#include "GameWorld.h"
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
            
            // Обновляем счёт в Application
            if (g_Application)
            {
                g_Application->AddGold(10);
                g_Application->AddChest();
            }
            
            SoundManager::GetInstance().PlaySound("chest");
            std::cout << "[Chest] Collected! +10 gold" << std::endl;
            
            // Удаляем сундук из мира
            GameWorld::GetInstance().DestroyGameObject(this);
        }
    }
}
