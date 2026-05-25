#pragma once
#include "Component.h"

namespace rogalique
{
    class TransformComponent : public Component
    {
    public:
        TransformComponent(RogaliqueGameObject* owner);
        void Update(float deltaTime) override;
        void Render(sf::RenderWindow& window) override;
        
        void SetPosition(const sf::Vector2f& pos);
        sf::Vector2f GetPosition() const { return m_position; }
        
    private:
        sf::Vector2f m_position;
    };
}
