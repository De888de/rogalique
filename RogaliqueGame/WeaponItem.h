#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

namespace rogalique {

    class WeaponItem {
    private:
        sf::RectangleShape m_weaponShape;
        sf::Vector2f m_position;
        bool m_isOnGround;
        float m_pickupRadius;
        float m_floatTime;

    public:
        WeaponItem(float x, float y);

        void Update(float dt);
        void Render(sf::RenderWindow& window);

        bool CheckPickup(sf::Vector2f playerPos);
        bool IsOnGround() const { return m_isOnGround; }
        sf::Vector2f GetPosition() const { return m_position; }
    };

}