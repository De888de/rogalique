#include "MovementComponent.h"
#include "TransformComponent.h"
#include "RogaliqueGameObject.h"
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
        if (transform)
        {
            sf::Vector2f pos = transform->GetPosition();
            pos += move * m_speed * deltaTime;
            
            // НЕ ОГРАНИЧИВАЕМ здесь — пусть доходит до краёв мира
            transform->SetPosition(pos);
        }
    }

    void MovementComponent::Render(sf::RenderWindow&) {}
}
