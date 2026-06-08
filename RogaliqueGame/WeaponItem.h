#pragma once
#include "Player.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

namespace rogalique {

class WeaponItem : public Player {
private:
    bool m_isOnGround;
    sf::RectangleShape m_weaponShape;
    float m_pickupRadius;
    float m_floatTime;
    
public:
    WeaponItem(float x, float z) : Player(), m_isOnGround(true), m_pickupRadius(50.0f), m_floatTime(0) {
        SetPosition(sf::Vector2f(x, z));
        
        m_weaponShape.setSize(sf::Vector2f(30, 20));
        m_weaponShape.setFillColor(sf::Color(148, 0, 211));
        m_weaponShape.setOrigin(15, 10);
        m_weaponShape.setPosition(x, z);
        
        std::cout << "[WeaponItem] Spawned at (" << x << ", " << z << ")" << std::endl;
    }

    void Update(float dt) override {
        if (!m_isOnGround) return;
        
        m_floatTime += dt;
        float offsetY = std::sin(m_floatTime * 3.0f) * 5.0f;
        m_weaponShape.setPosition(GetPosition().x, GetPosition().y + offsetY);
    }

    void Render(sf::RenderWindow& window) override {
        if (m_isOnGround) {
            window.draw(m_weaponShape);
        } else {
            Player::Render(window);
        }
    }

    bool CheckPickup(sf::Vector2f playerPos) {
        if (!m_isOnGround) return false;
        
        float dx = playerPos.x - GetPosition().x;
        float dy = playerPos.y - GetPosition().y;
        float dist = std::sqrt(dx*dx + dy*dy);
        
        if (dist < m_pickupRadius) {
            m_isOnGround = false;
            std::cout << "[WeaponItem] Picked up by player!" << std::endl;
            return true;
        }
        return false;
    }

    bool IsOnGround() const { return m_isOnGround; }
};

}
