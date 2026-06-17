#include "Portal.h"
#include <iostream>

namespace rogalique {

Portal::Portal() {
    m_shape.setSize(sf::Vector2f(40, 40));
    m_shape.setOrigin(20, 20);
    m_shape.setFillColor(sf::Color(0, 100, 255, 200));
    
    m_glowShape.setSize(sf::Vector2f(60, 60));
    m_glowShape.setOrigin(30, 30);
    m_glowShape.setFillColor(sf::Color(0, 50, 255, 50));
}

void Portal::Activate() {
    m_isActive = true;
    m_animationTimer = 0.0f;
    std::cout << "[Portal] Activated at (" << m_position.x << ", " << m_position.y << ")" << std::endl;
}

void Portal::Deactivate() {
    m_isActive = false;
}

void Portal::Update(float deltaTime) {
    if (m_isActive) {
        m_animationTimer += deltaTime * m_pulseSpeed;
    }
}

void Portal::Render(sf::RenderWindow& window) {
    if (!m_isActive) return;
    
    float pulse = 0.5f + 0.5f * std::sin(m_animationTimer);
    
    float glowSize = 60.0f + pulse * 20.0f;
    m_glowShape.setSize(sf::Vector2f(glowSize, glowSize));
    m_glowShape.setOrigin(glowSize/2, glowSize/2);
    m_glowShape.setFillColor(sf::Color(0, 50 + 100 * pulse, 255, 30 + 70 * pulse));
    m_glowShape.setPosition(m_position);
    window.draw(m_glowShape);
    
    m_shape.setFillColor(sf::Color(0, 100 + 100 * pulse, 255, 150 + 50 * pulse));
    m_shape.setPosition(m_position);
    window.draw(m_shape);
    
    sf::RectangleShape inner(sf::Vector2f(20, 20));
    inner.setOrigin(10, 10);
    inner.setFillColor(sf::Color(150, 200, 255, 100 + 50 * pulse));
    inner.setPosition(m_position);
    window.draw(inner);
}

void Portal::SetPosition(float x, float y) {
    m_position = sf::Vector2f(x, y);
}

bool Portal::IsNear(const sf::Vector2f& position, float radius) const {
    if (!m_isActive) return false;
    float dx = position.x - m_position.x;
    float dy = position.y - m_position.y;
    return std::sqrt(dx*dx + dy*dy) < radius;
}

}
