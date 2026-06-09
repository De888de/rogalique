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
        Bullet(sf::Vector2f position, sf::Vector2f direction, float speed = 500.0f)
            : m_active(true)
            , m_lifetime(0.0f)
        {
            // Добавляем компоненты
            auto* transform = AddComponent<TransformComponent>();
            if (transform) {
                transform->SetPosition(position);
            }
            
            AddComponent<CollisionComponent>(4.0f);  // Радиус коллизии 4 пикселя
            
            m_velocity = direction * speed;
            
            m_shape.setRadius(4.0f);
            m_shape.setFillColor(sf::Color(255, 100, 0));
            m_shape.setOrigin(4.0f, 4.0f);
            m_shape.setPosition(position);
            
            std::cout << "[Bullet] FIRE! Position: " << position.x << ", " << position.y << std::endl;
        }

        void Update(float deltaTime) override {
            if (!m_active) return;

            // Обновляем позицию через TransformComponent
            auto* transform = GetComponent<TransformComponent>();
            if (transform) {
                sf::Vector2f pos = transform->GetPosition();
                pos += m_velocity * deltaTime;
                transform->SetPosition(pos);
                m_shape.setPosition(pos);
            }

            m_lifetime += deltaTime;
            if (m_lifetime > 3.0f) { // Живёт 3 секунды
                m_active = false;
                auto& world = GameWorld::GetInstance();
                world.DestroyGameObject(this);
            }
        }

        void Render(sf::RenderWindow& window) override {
            if (m_active) {
                window.draw(m_shape);
            }
            
            // Рендер компонентов (если нужно)
            RogaliqueGameObject::Render(window);
        }

        bool IsActive() const { return m_active; }
        
        sf::Vector2f GetPosition() const {
            auto* transform = GetComponent<TransformComponent>();
            if (transform) return transform->GetPosition();
            return sf::Vector2f(0, 0);
        }

    private:
        sf::Vector2f m_velocity;
        sf::CircleShape m_shape;
        bool m_active;
        float m_lifetime;
    };
}
