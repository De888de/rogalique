#pragma once
#include "Component.h"
#include "RogaliqueGameObject.h"
#include <SFML/Graphics.hpp>

namespace rogalique
{
    class SeekerComponent : public Component
    {
    public:
        SeekerComponent(RogaliqueGameObject* owner, float speed);
        void Update(float deltaTime) override;
        void Render(sf::RenderWindow& window) override;

        void SetTarget(RogaliqueGameObject* target);
        RogaliqueGameObject* GetTarget() const { return m_target; }

    private:
        void TryMove(const sf::Vector2f& newPos, float deltaTime);
        void ChooseNewDirection();
        
        RogaliqueGameObject* m_target = nullptr;
        float m_speed;
        sf::Vector2f m_currentDirection;
        float m_stuckTimer = 0.0f;
        int m_stuckCounter = 0;
    };
}
