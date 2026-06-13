#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "RogaliqueGameObject.h"
#include "GameWorld.h"
#include "CollisionComponent.h"
#include "TransformComponent.h"

namespace rogalique {

    class Bullet : public RogaliqueGameObject {
    public:
        Bullet(sf::Vector2f position, sf::Vector2f direction, float speed, int damage)
            : m_active(true)
            , m_lifetime(0.0f)
            , m_damage(damage)
        {
            auto* transform = AddComponent<TransformComponent>();
            if (transform) {
                transform->SetPosition(position);
            }
            
            AddComponent<CollisionComponent>(4.0f);
            
            m_velocity = direction * speed;
            
            m_shape.setRadius(4.0f);
            m_shape.setFillColor(sf::Color(255, 100, 0));
            m_shape.setOrigin(4.0f, 4.0f);
            m_shape.setPosition(position);
            
            std::cout << "[Bullet] FIRE! Damage: " << m_damage << " | Position: " << position.x << ", " << position.y << std::endl;
        }

        void Update(float deltaTime) override {
            if (!m_active) return;

            auto* transform = GetComponent<TransformComponent>();
            if (transform) {
                sf::Vector2f pos = transform->GetPosition();
                pos += m_velocity * deltaTime;
                transform->SetPosition(pos);
                m_shape.setPosition(pos);
            }

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
            RogaliqueGameObject::Render(window);
        }

        bool IsActive() const { return m_active; }
        int GetDamage() const { return m_damage; }
        
        sf::Vector2f GetPosition() const {
            auto* transform = GetComponent<TransformComponent>();
            return transform ? transform->GetPosition() : sf::Vector2f(0, 0);
        }

    private:
        sf::Vector2f m_velocity;
        sf::CircleShape m_shape;
        bool m_active;
        float m_lifetime;
        int m_damage;
    };
}
