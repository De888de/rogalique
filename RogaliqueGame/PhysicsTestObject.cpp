#include "PhysicsTestObject.h"
#include "RigidbodyComponent.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include <iostream>

namespace rogalique {

PhysicsTestObject::PhysicsTestObject() {
    m_transform = AddComponent<TransformComponent>();
    m_rigidbody = AddComponent<RigidbodyComponent>();

    // Настройки физики
    m_rigidbody->SetMass(0.5f);
    m_rigidbody->SetGravityScale(1.0f);
    m_rigidbody->SetBounciness(0.7f);

    // Визуал (запасной вариант)
    m_shape.setRadius(12.0f);
    m_shape.setFillColor(sf::Color::Red);
    m_shape.setOrigin(12.0f, 12.0f);
    m_useShape = true;
}

void PhysicsTestObject::Spawn(float x, float y) {
    if (m_transform) {
        m_transform->SetPosition(sf::Vector2f(x, y));
        m_shape.setPosition(x, y);
    }

    // Случайная начальная скорость
    float vx = (rand() % 400) - 200;
    float vy = (rand() % 300) - 400;
    if (m_rigidbody) {
        m_rigidbody->SetVelocity(vx, vy);
        // Принудительно включаем гравитацию
        m_rigidbody->SetGravityScale(1.0f);
        m_rigidbody->SetLinearDamping(0.98f);
    }

    std::cout << "PhysicsTestObject spawned at (" << x << ", " << y
              << ") velocity: (" << vx << ", " << vy << ")" << std::endl;
}

void PhysicsTestObject::ApplyForce(float x, float y) {
    if (m_rigidbody) {
        m_rigidbody->AddForce(sf::Vector2f(x, y));
    }
}

void PhysicsTestObject::ApplyImpulse(float x, float y) {
    if (m_rigidbody) {
        m_rigidbody->AddImpulse(sf::Vector2f(x, y));
    }
}

void PhysicsTestObject::Update(float deltaTime) {
    if (!m_transform || !m_rigidbody) return;
    
    // ===== ВАЖНО: обновляем физику =====
    m_rigidbody->Update(deltaTime);
    // ==================================
    
    sf::Vector2f pos = m_transform->GetPosition();
    sf::Vector2f vel = m_rigidbody->GetVelocity();

    // Отскок от границ экрана
    if (pos.x < 20 && vel.x < 0) {
        m_rigidbody->SetVelocity(-vel.x * 0.6f, vel.y);
        pos.x = 20;
    }
    if (pos.x > 780 && vel.x > 0) {
        m_rigidbody->SetVelocity(-vel.x * 0.6f, vel.y);
        pos.x = 780;
    }
    if (pos.y < 20 && vel.y < 0) {
        m_rigidbody->SetVelocity(vel.x, -vel.y * 0.6f);
        pos.y = 20;
    }
    if (pos.y > 580 && vel.y > 0) {
        m_rigidbody->SetVelocity(vel.x, -vel.y * 0.6f);
        pos.y = 580;
    }

    m_transform->SetPosition(pos);
    m_shape.setPosition(pos);
}

void PhysicsTestObject::Render(sf::RenderWindow& window) {
    if (m_useShape) {
        window.draw(m_shape);
    }
}

} // namespace rogalique
