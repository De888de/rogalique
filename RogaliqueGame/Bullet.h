#pragma once
#include <SFML/Graphics.hpp>

namespace rogalique {

class Bullet {
private:
    sf::CircleShape m_shape;
    sf::Vector2f m_pos;
    sf::Vector2f m_vel;
    bool m_active;
    float m_life;
    
public:
    Bullet(sf::Vector2f pos, sf::Vector2f dir) {
        m_pos = pos;
        m_vel = dir * 500.0f;
        m_active = true;
        m_life = 2.0f;
        
        m_shape.setRadius(4);
        m_shape.setFillColor(sf::Color::Yellow);
        m_shape.setOrigin(4, 4);
        m_shape.setPosition(m_pos);
    }
    
    void Update(float dt) {
        if (!m_active) return;
        m_pos += m_vel * dt;
        m_life -= dt;
        m_shape.setPosition(m_pos);
        if (m_life <= 0) m_active = false;
    }
    
    void Render(sf::RenderWindow& w) {
        if (m_active) w.draw(m_shape);
    }
    
    bool IsActive() { return m_active; }
};

}
