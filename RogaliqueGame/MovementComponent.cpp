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
        static int frame = 0;
        frame++;
        // Выводим каждые 60 кадров
        if (frame % 60 == 1)
            std::cout << "[Movement] Update called, frame=" << frame << std::endl;
        
        sf::Vector2f move(0, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) move.y -= 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) move.y += 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) move.x -= 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) move.x += 1;
        
        if (move.x != 0 || move.y != 0)
        {
            std::cout << "[Movement] Key pressed! W=" << sf::Keyboard::isKeyPressed(sf::Keyboard::W)
                      << " S=" << sf::Keyboard::isKeyPressed(sf::Keyboard::S)
                      << " A=" << sf::Keyboard::isKeyPressed(sf::Keyboard::A)
                      << " D=" << sf::Keyboard::isKeyPressed(sf::Keyboard::D) << std::endl;
        }

        auto* transform = m_owner->GetComponent<TransformComponent>();
        if (transform)
        {
            sf::Vector2f pos = transform->GetPosition();
            pos += move * m_speed * deltaTime;
            
            const float limit = 20.0f;
            if (pos.x < limit) pos.x = limit;
            if (pos.x > 1024 - limit) pos.x = 1024 - limit;
            if (pos.y < limit) pos.y = limit;
            if (pos.y > 768 - limit) pos.y = 768 - limit;
            
            transform->SetPosition(pos);
        }
        else
        {
            static bool warned = false;
            if (!warned)
            {
                std::cout << "[Movement] ERROR: No TransformComponent found!" << std::endl;
                warned = true;
            }
        }
    }

    void MovementComponent::Render(sf::RenderWindow&) {}
}
