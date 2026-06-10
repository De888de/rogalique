#include "Enemy.h"
#include "Bullet.h"
#include "Logger.h"
#include "SoundManager.h"
#include "Application.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include "SeekerComponent.h"
#include "GameWorld.h"
#include <iostream>

namespace rogalique
{
    Enemy::Enemy()
    {
        AddComponent<TransformComponent>();
        AddComponent<SpriteComponent>("enemy.png", 32, 32);
        AddComponent<SeekerComponent>(80.0f);
        AddComponent<CollisionComponent>(16.0f);
        
        // Загружаем шрифт для отображения HP
        if (!m_font.loadFromFile("D:/xyz/roqalique/RogaliqueGame/Resources/Fonts/Roboto-Regular.ttf")) {
            std::cout << "[Enemy] Could not load font" << std::endl;
        }
        
        m_healthText.setFont(m_font);
        m_healthText.setCharacterSize(16);
        m_healthText.setFillColor(sf::Color::White);
        UpdateUIText();
        
        std::cout << "[Enemy] Created with " << m_health << " HP" << std::endl;
    }

    void Enemy::Update(float deltaTime)
    {
        RogaliqueGameObject::Update(deltaTime);
        
        if (m_health <= 0)
        {
            std::cout << "[Enemy] Died!" << std::endl;
            GameWorld::GetInstance().DestroyGameObject(this);
        }
        
        // Обновляем позицию текста HP
        auto* transform = GetComponent<TransformComponent>();
        if (transform) {
            sf::Vector2f pos = transform->GetPosition();
            m_healthText.setPosition(pos.x - 20, pos.y - 25);
        }
    }
    
    void Enemy::Render(sf::RenderWindow& window)
    {
        RogaliqueGameObject::Render(window);
        if (m_health > 0) {
            window.draw(m_healthText);
        }
    }

    void Enemy::TakeDamage(int damage)
    {
        if (m_health <= 0) return;

        if (g_Application) {
            SoundManager::GetInstance().PlaySound("monster_hit");
        }
        
        m_health -= damage;
        if (m_health < 0) m_health = 0;
        
        std::cout << "[Enemy] Took " << damage << " damage! HP: " << m_health << "/" << m_maxHealth << std::endl;
        UpdateUIText();

        LOG_ENEMY_DEATH("Basic Enemy");
    }

    void Enemy::Heal(int amount)
    {
        m_health += amount;
        if (m_health > m_maxHealth) m_health = m_maxHealth;
        UpdateUIText();
    }
    
    void Enemy::UpdateUIText()
    {
        int percent = (m_health * 100) / m_maxHealth;
        m_healthText.setString(std::to_string(percent) + "%");
    }
    
    sf::Vector2f Enemy::GetPosition() const
    {
        auto* transform = GetComponent<TransformComponent>();
        if (transform) return transform->GetPosition();
        return sf::Vector2f(0, 0);
    }
}
