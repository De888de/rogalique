#pragma once
#include <SFML/Graphics.hpp>

namespace rogalique
{
    class RogaliqueGameObject
    {
    public:
        RogaliqueGameObject();
        virtual ~RogaliqueGameObject() = default;
        
        virtual void Update(float deltaTime) = 0;
        virtual void Draw(sf::RenderWindow& window);
        
        void SetPosition(const sf::Vector2f& pos) { m_position = pos; }
        sf::Vector2f GetPosition() const { return m_position; }
        sf::FloatRect GetBounds() const;
        
        bool IsActive() const { return m_isActive; }
        void SetActive(bool active) { m_isActive = active; }
        
    protected:
        sf::Vector2f m_position;
        bool m_isActive;
    };
}
