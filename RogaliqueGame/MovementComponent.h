#pragma once
#include "Component.h"

namespace rogalique
{
    class MovementComponent : public Component
    {
    public:
        MovementComponent(RogaliqueGameObject* owner, float speed);
        void Update(float deltaTime) override;
        void Render(sf::RenderWindow& window) override;
        
    private:
        float m_speed;
    };
}
