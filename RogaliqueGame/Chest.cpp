#include "Chest.h"
#include "TransformComponent.h"
#include "SoundManager.h"
#include "Application.h"
#include "GameWorld.h"
#include <iostream>

namespace rogalique
{
    Chest::Chest()
    {
        // Добавляем TransformComponent для позиции
        AddComponent<TransformComponent>();
        
        m_shape.setSize(sf::Vector2f(32, 32));
        m_shape.setFillColor(sf::Color(255, 215, 0)); // золотой
        m_shape.setOrigin(16, 16);
    }
    
    void Chest::Update(float deltaTime)
    {
        (void)deltaTime;
        
        // Обновляем позицию из TransformComponent
        auto* transform = GetComponent<TransformComponent>();
        if (transform) {
            m_shape.setPosition(transform->GetPosition());
        }
    }
    
    void Chest::Render(sf::RenderWindow& window)
    {
        if (!m_isCollected)
        {
            window.draw(m_shape);
        }
    }
    
    void Chest::Collect()
    {
        if (!m_isCollected)
        {
            m_isCollected = true;
            
            if (g_Application)
            {
                g_Application->AddGold(10);
                g_Application->AddChest();
            }
            
            SoundManager::GetInstance().PlaySound("chest");
            std::cout << "[Chest] Collected! +10 gold" << std::endl;
            
            GameWorld::GetInstance().DestroyGameObject(this);
        }
    }
    
    sf::Vector2f Chest::GetPosition() const
    {
        auto* transform = GetComponent<TransformComponent>();
        return transform ? transform->GetPosition() : sf::Vector2f(0, 0);
    }
}
