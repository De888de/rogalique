#include "Player.h"
#include <SFML/Window/Keyboard.hpp>
#include <cmath>
#include <algorithm>

namespace rogalique
{
    Player::Player()
    {
        m_shape.setSize(sf::Vector2f(32, 32));
        m_shape.setFillColor(sf::Color::Green);
        m_shape.setOrigin(16, 16);
    }

    void Player::Update(float deltaTime)
    {
        sf::Vector2f move(0, 0);
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) move.y -= 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) move.y += 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) move.x -= 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) move.x += 1;
        
        if (move.x != 0 || move.y != 0)
        {
            float len = std::sqrt(move.x * move.x + move.y * move.y);
            move /= len;
        }
        
        m_position += move * m_speed * deltaTime;
        
        float halfSize = 16.0f;
        m_position.x = std::clamp(m_position.x, halfSize, 1024.0f - halfSize);
        m_position.y = std::clamp(m_position.y, halfSize, 768.0f - halfSize);
        
        m_shape.setPosition(m_position);
    }

    void Player::Draw(sf::RenderWindow& window)
    {
        window.draw(m_shape);
    }

    void Player::TakeDamage(int damage)
    {
        m_health = std::max(0, m_health - damage);
    }

    void Player::Heal(int amount)
    {
        m_health = std::min(m_maxHealth, m_health + amount);
    }
}
