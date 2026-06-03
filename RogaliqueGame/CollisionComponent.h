#pragma once
#include "Component.h"
#include "RogaliqueGameObject.h"
#include <SFML/Graphics.hpp>
#include <vector>

namespace rogalique
{
    class CollisionComponent : public Component
    {
    public:
        CollisionComponent(RogaliqueGameObject* owner, float radius);
        void Update(float deltaTime) override;
        void Render(sf::RenderWindow& window) override;
        
        bool CheckCollision(CollisionComponent* other);
        sf::FloatRect GetBounds() const;
        void OnCollisionEnter(CollisionComponent* other);
        void OnCollisionExit(CollisionComponent* other);
        
        void SetRadius(float radius) { m_radius = radius; }
        float GetRadius() const { return m_radius; }
        
        bool IsTrigger() const { return m_isTrigger; }
        void SetTrigger(bool trigger) { m_isTrigger = trigger; }
        
        const std::vector<CollisionComponent*>& GetCollidingObjects() const { return m_collidingObjects; }
        
    private:
        float m_radius;
        bool m_isTrigger;
        std::vector<CollisionComponent*> m_collidingObjects;
    };
}
