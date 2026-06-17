#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

namespace rogalique {

class Portal {
public:
    Portal();
    void Activate();
    void Deactivate();
    void Update(float deltaTime);
    void Render(sf::RenderWindow& window);
    void SetPosition(float x, float y);
    
    bool IsActive() const { return m_isActive; }
    bool IsNear(const sf::Vector2f& position, float radius = 35.0f) const;
    sf::Vector2f GetPosition() const { return m_position; }

private:
    sf::Vector2f m_position;
    sf::RectangleShape m_shape;
    sf::RectangleShape m_glowShape;
    bool m_isActive = false;
    float m_animationTimer = 0.0f;
    float m_pulseSpeed = 2.0f;
};

}
