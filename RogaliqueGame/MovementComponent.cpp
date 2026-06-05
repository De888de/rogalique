#include <cmath>
#include "MovementComponent.h"
#include "TransformComponent.h"
#include "CollisionComponent.h"
#include "GameWorld.h"
#include <cmath>
#include <iostream>

namespace rogalique
{
    MovementComponent::MovementComponent(RogaliqueGameObject* owner, float speed)
        : Component(owner), m_speed(speed) 
    {
        std::cout << "[Movement] Created, speed=" << m_speed << std::endl;
    }

    void MovementComponent::Update(float deltaTime)
    {
        sf::Vector2f move(0, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) move.y -= 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) move.y += 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) move.x -= 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) move.x += 1;

        if (move.x != 0 || move.y != 0)
        {
            float len = std::sqrt(move.x * move.x + move.y * move.y);
            move /= len;
        }

        auto* transform = m_owner->GetComponent<TransformComponent>();
        if (!transform) return;
        
        sf::Vector2f oldPos = transform->GetPosition();
        sf::Vector2f newPos = oldPos + move * m_speed * deltaTime;
        
        // Временно устанавливаем новую позицию
        transform->SetPosition(newPos);
        
        // Проверяем коллизию
        auto* myCollision = m_owner->GetComponent<CollisionComponent>();
        if (myCollision)
        {
            auto& world = GameWorld::GetInstance();
            bool hasCollision = false;
            
            for (auto* obj : world.GetAllGameObjects())
            {
                if (obj == m_owner) continue;
                
                auto* otherCollision = obj->GetComponent<CollisionComponent>();
                if (otherCollision && myCollision->CheckCollision(otherCollision))
                {
                    hasCollision = true;
                    break;
                }
            }
            
            if (hasCollision)
            {
                std::cout << "[Movement] Collision detected! Reverting position" << std::endl;
                transform->SetPosition(oldPos);
            }
        }
    }

    void MovementComponent::Render(sf::RenderWindow&) {}
}

