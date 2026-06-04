#include "SeekerComponent.h"
#include "TransformComponent.h"
#include "CollisionComponent.h"
#include "GameWorld.h"
#include "Player.h"
#include <iostream>
#include <cmath>
#include <random>

namespace rogalique
{
    SeekerComponent::SeekerComponent(RogaliqueGameObject* owner, float speed)
        : Component(owner), m_speed(speed)
    {
        auto& world = GameWorld::GetInstance();
        for (auto* obj : world.GetAllGameObjects())
        {
            if (dynamic_cast<Player*>(obj))
            {
                m_target = obj;
                std::cout << "[SeekerComponent] Target acquired: Player" << std::endl;
                break;
            }
        }
        m_currentDirection = sf::Vector2f(1, 0);
    }

    void SeekerComponent::TryMove(const sf::Vector2f& newPos, float deltaTime)
    {
        auto* transform = m_owner->GetComponent<TransformComponent>();
        if (!transform) return;
        
        sf::Vector2f oldPos = transform->GetPosition();
        transform->SetPosition(newPos);
        
        auto* myCollision = m_owner->GetComponent<CollisionComponent>();
        if (myCollision)
        {
            auto& world = GameWorld::GetInstance();
            for (auto* obj : world.GetAllGameObjects())
            {
                if (obj == m_owner) continue;
                
                auto* otherCollision = obj->GetComponent<CollisionComponent>();
                if (otherCollision && myCollision->CheckCollision(otherCollision))
                {
                    // Столкновение! Откатываем и меняем направление
                    transform->SetPosition(oldPos);
                    ChooseNewDirection();
                    return;
                }
            }
        }
        
        // Успешно переместились
        m_stuckCounter = 0;
        m_stuckTimer = 0.0f;
    }

    void SeekerComponent::ChooseNewDirection()
    {
        m_stuckCounter++;
        
        // Случайный поворот влево или вправо
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> dis(0, 1);
        
        float angle = (dis(gen) == 0) ? 90.0f : -90.0f;
        float rad = angle * 3.14159f / 180.0f;
        
        float newX = m_currentDirection.x * std::cos(rad) - m_currentDirection.y * std::sin(rad);
        float newY = m_currentDirection.x * std::sin(rad) + m_currentDirection.y * std::cos(rad);
        
        m_currentDirection = sf::Vector2f(newX, newY);
        
        // Нормализация
        float len = std::sqrt(m_currentDirection.x * m_currentDirection.x + m_currentDirection.y * m_currentDirection.y);
        if (len > 0.01f)
            m_currentDirection /= len;
            
        std::cout << "[SeekerComponent] Stuck! New direction: (" << m_currentDirection.x << ", " << m_currentDirection.y << ")" << std::endl;
    }

    void SeekerComponent::Update(float deltaTime)
    {
        if (!m_target) return;

        auto* ownerTransform = m_owner->GetComponent<TransformComponent>();
        auto* targetTransform = m_target->GetComponent<TransformComponent>();

        if (!ownerTransform || !targetTransform) return;

        sf::Vector2f ownerPos = ownerTransform->GetPosition();
        sf::Vector2f targetPos = targetTransform->GetPosition();

        // Идеальное направление к игроку
        sf::Vector2f idealDirection = targetPos - ownerPos;
        float len = std::sqrt(idealDirection.x * idealDirection.x + idealDirection.y * idealDirection.y);
        if (len > 0.01f)
            idealDirection /= len;
        
        // Плавно поворачиваем к игроку (если не застряли)
        if (m_stuckCounter < 3)
        {
            m_currentDirection = m_currentDirection + (idealDirection - m_currentDirection) * 0.1f;
            len = std::sqrt(m_currentDirection.x * m_currentDirection.x + m_currentDirection.y * m_currentDirection.y);
            if (len > 0.01f)
                m_currentDirection /= len;
        }
        
        sf::Vector2f newPos = ownerPos + m_currentDirection * m_speed * deltaTime;
        TryMove(newPos, deltaTime);
    }

    void SeekerComponent::Render(sf::RenderWindow&) {}

    void SeekerComponent::SetTarget(RogaliqueGameObject* target)
    {
        m_target = target;
    }
}
