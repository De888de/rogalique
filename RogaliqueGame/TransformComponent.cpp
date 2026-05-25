#include "TransformComponent.h"
#include <iostream>

namespace rogalique
{
    TransformComponent::TransformComponent(RogaliqueGameObject* owner)
        : Component(owner), m_position(0, 0) {}
    
    void TransformComponent::Update(float) {}
    void TransformComponent::Render(sf::RenderWindow&) {}
    
    void TransformComponent::SetPosition(const sf::Vector2f& pos)
    {
        static int callCount = 0;
        if (callCount++ % 60 == 0)
            std::cout << "[Transform] SetPosition: (" << pos.x << "," << pos.y << ")" << std::endl;
        m_position = pos;
    }
}
