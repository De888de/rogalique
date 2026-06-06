#include "IsometricPhysicsItem.h"
#include "TransformComponent.h"
#include "CollisionComponent.h"
#include "GameWorld.h"
#include "Application.h"
#include <iostream>
#include <cmath>

namespace rogalique {

static const float ISO_ANGLE = 0.523599f;
static const float COS_ANGLE = std::cos(ISO_ANGLE);
static const float SIN_ANGLE = std::sin(ISO_ANGLE);

IsometricPhysicsItem::IsometricPhysicsItem() {
    m_transform = AddComponent<TransformComponent>();
    m_collision = AddComponent<CollisionComponent>(m_radius);
    
    m_shape.setRadius(m_radius);
    m_shape.setFillColor(sf::Color::Red);
    m_shape.setOrigin(m_radius, m_radius);
    
    m_shadow.setRadius(m_radius);
    m_shadow.setFillColor(sf::Color(0, 0, 0, 100));
    m_shadow.setOrigin(m_radius, m_radius);
}

sf::Vector2f IsometricPhysicsItem::ScreenToWorld(float screenX, float screenY) const {
    float cx = screenX - SCREEN_WIDTH / 2.0f;
    float cy = screenY - SCREEN_HEIGHT / 2.0f;
    float worldX = (cx / COS_ANGLE + cy / SIN_ANGLE) / 2.0f;
    float worldZ = (cy / SIN_ANGLE - cx / COS_ANGLE) / 2.0f;
    return sf::Vector2f(worldX, worldZ);
}

sf::Vector2f IsometricPhysicsItem::WorldToScreen(const sf::Vector2f& world, float height) const {
    float screenX = (world.x - world.y) * COS_ANGLE;
    float screenY = (world.x + world.y) * SIN_ANGLE - height;
    screenX += SCREEN_WIDTH / 2.0f;
    screenY += SCREEN_HEIGHT / 2.0f;
    return sf::Vector2f(screenX, screenY);
}

void IsometricPhysicsItem::Spawn(float worldX, float worldZ) {
    m_worldPos = sf::Vector2f(worldX, worldZ);
    m_height = 0.0f;
    m_velGround = sf::Vector2f((rand() % 400) - 200, (rand() % 400) - 200);
    m_velY = (rand() % 500) + 200;
    
    sf::Vector2f screenPos = WorldToScreen(m_worldPos, m_height);
    if (m_transform) m_transform->SetPosition(screenPos);
    m_active = true;
}

void IsometricPhysicsItem::ApplyImpulse(float velX, float velZ, float velY) {
    m_velGround.x += velX;
    m_velGround.y += velZ;
    m_velY += velY;
}

void IsometricPhysicsItem::HandleCollisions() {
    auto& world = GameWorld::GetInstance();
    auto& objects = world.GetAllGameObjects();
    
    for (auto* obj : objects) {
        if (obj == this) continue;
        
        auto* otherCollision = obj->GetComponent<CollisionComponent>();
        if (otherCollision && m_collision && m_collision->CheckCollision(otherCollision)) {
            sf::Vector2f delta = m_transform->GetPosition() - obj->GetPosition();
            float len = std::sqrt(delta.x * delta.x + delta.y * delta.y);
            if (len > 0.01f) delta /= len;
            
            float dot = m_velGround.x * delta.x + m_velGround.y * delta.y;
            m_velGround.x = (m_velGround.x - dot * delta.x) * 0.7f;
            m_velGround.y = (m_velGround.y - dot * delta.y) * 0.7f;
            
            float overlap = (m_radius + 20.0f) - len;
            m_worldPos += sf::Vector2f(delta.x * overlap, delta.y * overlap);
            
            std::cout << "Collision with object!" << std::endl;
        }
    }
}

void IsometricPhysicsItem::Update(float deltaTime) {
    if (!m_active) return;
    
    m_velY -= m_gravity * deltaTime;
    m_height += m_velY * deltaTime;
    
    if (m_height < 0) {
        m_height = 0;
        m_velY = -m_velY * m_bounciness;
        if (std::abs(m_velY) < 50.0f) m_velY = 0;
    }
    
    m_velGround *= m_damping;
    m_worldPos += m_velGround * deltaTime;
    
    HandleCollisions();
    
    float left = m_radius;
    float right = WORLD_WIDTH - m_radius;
    float top = m_radius;
    float bottom = WORLD_HEIGHT - m_radius;
    
    if (m_worldPos.x < left && m_velGround.x < 0) {
        m_velGround.x = -m_velGround.x * 0.5f;
        m_worldPos.x = left;
    }
    if (m_worldPos.x > right && m_velGround.x > 0) {
        m_velGround.x = -m_velGround.x * 0.5f;
        m_worldPos.x = right;
    }
    if (m_worldPos.y < top && m_velGround.y < 0) {
        m_velGround.y = -m_velGround.y * 0.5f;
        m_worldPos.y = top;
    }
    if (m_worldPos.y > bottom && m_velGround.y > 0) {
        m_velGround.y = -m_velGround.y * 0.5f;
        m_worldPos.y = bottom;
    }
    
    sf::Vector2f screenPos = WorldToScreen(m_worldPos, m_height);
    if (m_transform) m_transform->SetPosition(screenPos);
    m_shape.setPosition(screenPos);
    m_shadow.setPosition(WorldToScreen(m_worldPos, 0));
    
    if (m_worldPos.x < -500 || m_worldPos.x > WORLD_WIDTH + 500 ||
        m_worldPos.y < -500 || m_worldPos.y > WORLD_HEIGHT + 500) {
        m_active = false;
    }
}

void IsometricPhysicsItem::Render(sf::RenderWindow& window) {
    if (m_active) {
        window.draw(m_shadow);
        window.draw(m_shape);
    }
}

} // namespace rogalique
