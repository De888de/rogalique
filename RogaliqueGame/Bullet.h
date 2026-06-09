#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "RogaliqueGameObject.h"
#include "GameWorld.h"

namespace rogalique {

    class Bullet : public RogaliqueGameObject {
    public:
        Bullet(sf::Vector2f position, sf::Vector2f direction, float speed = 500.0f)
            : m_position(position)
            , m_velocity(direction * speed)
            , m_active(true)
            , m_lifetime(0.0f)
        {
            m_shape.setRadius(4.0f);
            m_shape.setFillColor(sf::Color(255, 100, 0));
            m_shape.setOrigin(4.0f, 4.0f);
            m_shape.setPosition(position);
            std::cout << "[Bullet] FIRE! Position: " << position.x << ", " << position.y << std::endl;
        }

        void Update(float deltaTime) override {
            if (!m_active) return;

            m_position += m_velocity * deltaTime;
            m_shape.setPosition(m_position);

            m_lifetime += deltaTime;
            if (m_lifetime > 3.0f) {
                m_active = false;
                auto& world = GameWorld::GetInstance();
                world.DestroyGameObject(this);
            }
        }

        void Render(sf::RenderWindow& window) override {
            if (m_active) {
                window.draw(m_shape);
            }
        }

        bool IsActive() const { return m_active; }
        sf::Vector2f GetPosition() const { return m_position; }

    private:
        sf::Vector2f m_position;
        sf::Vector2f m_velocity;
        sf::CircleShape m_shape;
        bool m_active;
        float m_lifetime;
    };
}
