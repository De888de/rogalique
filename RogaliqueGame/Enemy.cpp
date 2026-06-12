#include "Enemy.h"
#include "Bullet.h"
#include "Logger.h"
#include "SoundManager.h"
#include "Application.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include "SeekerComponent.h"
#include "AttackComponent.h"
#include "GameWorld.h"
#include <iostream>
#include <cassert>



namespace rogalique
{
    Enemy::Enemy()
    {
        AddComponent<TransformComponent>();
        AddComponent<SpriteComponent>("enemy.png", 32, 32);
        AddComponent<SeekerComponent>(80.0f);
        AddComponent<CollisionComponent>(16.0f);
        
        // Добавляем HealthComponent
        m_healthComponent = AddComponent<HealthComponent>(100);

        AddComponent<AttackComponent>(1, 40.0f, 1.0f);
        
        // Загружаем шрифт для отображения HP
        if (!m_font.loadFromFile("RogaliqueGame/Resources/Fonts/Roboto-Regular.ttf")) {
            std::cout << "[Enemy] Could not load font" << std::endl;
        }
        
        m_healthText.setFont(m_font);
        m_healthText.setCharacterSize(16);
        m_healthText.setFillColor(sf::Color::White);
        UpdateUIText();

        
        class AttackComponent;

        AttackComponent* m_attackComponent = nullptr;
        
        std::cout << "[Enemy] Created with " << GetHealth() << " HP" << std::endl;
    }

    void Enemy::Update(float deltaTime)
    {
        RogaliqueGameObject::Update(deltaTime);
        
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
        if (IsAlive()) {
            window.draw(m_healthText);
        }
    }

    int Enemy::GetHealth() const
    {
        return m_healthComponent ? m_healthComponent->GetHealth() : 0;
    }

    int Enemy::GetMaxHealth() const
    {
        return m_healthComponent ? m_healthComponent->GetMaxHealth() : 0;
    }

    bool Enemy::IsAlive() const
    {
        return m_healthComponent && m_healthComponent->IsAlive();
    }

    void Enemy::TakeDamage(int damage)
    {
        assert(damage > 0 && "Damage must be positive");
        assert(m_healthComponent && "Enemy must have HealthComponent");

        if (!IsAlive()) return;

        if (g_Application) {
            SoundManager::GetInstance().PlaySound("monster_hit");
        }
        
        int oldHealth = GetHealth();
        m_healthComponent->TakeDamage(damage);
        int newHealth = GetHealth();
        
        std::cout << "[Enemy] Took " << damage << " damage! HP: " << newHealth << "/" << GetMaxHealth() << std::endl;
        UpdateUIText();

        // ЛОГ СМЕРТИ ВРАГ УМИРАЕТ
        if (!IsAlive()) {
            LOG_ENEMY_DEATH("Basic Enemy");
            std::cout << "[Enemy] Died!" << std::endl;
            GameWorld::GetInstance().DestroyGameObject(this);
        }
    }

    void Enemy::Heal(int amount)
    {
        if (!m_healthComponent) return;
        m_healthComponent->Heal(amount);
        UpdateUIText();
    }
    
    void Enemy::UpdateUIText()
    {
        int percent = (GetHealth() * 100) / GetMaxHealth();
        m_healthText.setString(std::to_string(percent) + "%");
    }
    
    sf::Vector2f Enemy::GetPosition() const
    {
        auto* transform = GetComponent<TransformComponent>();
        return transform ? transform->GetPosition() : sf::Vector2f(0, 0);
    }
}
