#include <cmath>
#include "CollisionComponent.h"
#include "Player.h"
#include "Chest.h"
#include "GameWorld.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "MovementComponent.h"
#include "HealthComponent.h"
#include <cmath>
#include <iostream>

namespace rogalique
{
    Player::Player()
    {
        AddComponent<TransformComponent>();
        AddComponent<SpriteComponent>("player.png", 32, 32);
        AddComponent<MovementComponent>(200.0f);
        AddComponent<HealthComponent>(100);
        AddComponent<CollisionComponent>(16.0f);

        // Инициализация оружия
        m_equippedWeapon.setSize(sf::Vector2f(30, 20));
        m_equippedWeapon.setFillColor(sf::Color(148, 0, 211));
        m_equippedWeapon.setOrigin(5, 10); // чуть смещено вправо от игрока
    }

    void Player::EquipWeapon()
    {
        if (!m_hasWeapon) {
            m_hasWeapon = true;
            std::cout << "[Player] Weapon equipped!" << std::endl;
        }
    }

    void Player::Render(sf::RenderWindow& window)
    {
        // Сначала рендерим базовые компоненты
        RogaliqueGameObject::Render(window);

        // Рендерим оружие в руке
        if (m_hasWeapon)
        {
            auto* transform = GetComponent<TransformComponent>();
            if (transform)
            {
                sf::Vector2f pos = transform->GetPosition();
                // Оружие немного справа и чуть выше от центра игрока
                m_equippedWeapon.setPosition(pos.x + 18, pos.y - 5);
                window.draw(m_equippedWeapon);
            }
        }
    }
    
    void Player::Update(float deltaTime)
    {
        RogaliqueGameObject::Update(deltaTime);
        
        // Получаем позицию из TransformComponent
        auto* transform = GetComponent<TransformComponent>();
        if (!transform) return;
        
        sf::Vector2f playerPos = transform->GetPosition();
        
        // Проверка сбора сундуков
        auto& world = GameWorld::GetInstance();
        for (auto* obj : world.GetAllGameObjects())
        {
            auto* chest = dynamic_cast<Chest*>(obj);
            if (chest && !chest->IsCollected())
            {
                float dx = playerPos.x - chest->GetPosition().x;
                float dy = playerPos.y - chest->GetPosition().y;
                float dist = std::sqrt(dx*dx + dy*dy);
                if (dist < 32.0f)
                {
                    chest->Collect();
                }
            }
        }
    }
    sf::Vector2f Player::GetPosition() const
    {
        auto* transform = GetComponent<TransformComponent>();
        if (transform)
            return transform->GetPosition();
        return sf::Vector2f(0, 0);
    }

   

    
}
