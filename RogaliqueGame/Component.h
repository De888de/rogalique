#pragma once
#include <SFML/Graphics.hpp>

namespace rogalique
{
    class RogaliqueGameObject;
    
    class Component
    {
    public:
        Component(RogaliqueGameObject* owner);
        virtual ~Component() = default;
        
        virtual void Update(float deltaTime) = 0;
        virtual void Render(sf::RenderWindow& window) = 0;
        
        RogaliqueGameObject* GetOwner() const { return m_owner; }
        
    protected:
        RogaliqueGameObject* m_owner;
    };
}
