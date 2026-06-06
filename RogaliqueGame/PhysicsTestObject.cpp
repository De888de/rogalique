#include "PhysicsTestObject.h"
#include "RigidbodyComponent.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "Application.h"
#include <iostream>

namespace rogalique {

PhysicsTestObject::PhysicsTestObject() {
    m_transform = AddComponent<TransformComponent>();
    m_rigidbody = AddComponent<RigidbodyComponent>();
    
    m_rigidbody->SetMass(0.5f);
    m_rigidbody->SetGravityScale(1.0f);
    m_rigidbody->SetBounciness(0.7f);
    
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
    
    float vx = (rand() % 400) - 200;
    float vy = (rand() % 300) - 400;
    if (m_rigidbody) {
        m_rigidbody->SetVelocity(vx, vy);
    }
    
    std::cout << "PhysicsTestObject spawned at (" << x << ", " << y 
              << ") velocity: (" << vx << ", " << vy << ")" << std::endl;
}

void PhysicsTestObject::Update(float deltaTime) {
    if (!m_transform || !m_rigidbody) return;
    
    m_rigidbody->Update(deltaTime);
    
    sf::Vector2f pos = m_transform->GetPosition();
    sf::Vector2f vel = m_rigidbody->GetVelocity();
    float radius = 12.0f;
    
    // ГРАНИЦЫ МИРА (а не экрана)
    float left = radius;
    float right = WORLD_WIDTH - radius;
    float top = radius;
    float bottom = WORLD_HEIGHT - radius;
    
    // Отскок от границ мира
    if (pos.x < left && vel.x < 0) {
        m_rigidbody->SetVelocity(-vel.x * 0.6f, vel.y);
        pos.x = left;
    }
    if (pos.x > right && vel.x > 0) {
        m_rigidbody->SetVelocity(-vel.x * 0.6f, vel.y);
        pos.x = right;
    }
    if (pos.y < top && vel.y < 0) {
        m_rigidbody->SetVelocity(vel.x, -vel.y * 0.6f);
        pos.y = top;
    }
    if (pos.y > bottom && vel.y > 0) {
        m_rigidbody->SetVelocity(vel.x, -vel.y * 0.6f);
        pos.y = bottom;
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
