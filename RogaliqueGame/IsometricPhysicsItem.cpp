#include "IsometricPhysicsItem.h"
#include "TransformComponent.h"
#include "CollisionComponent.h"
#include "GameWorld.h"
#include "Application.h"
#include "Player.h"                
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

        m_shadow.setRadius(m_radius * 1.1f);
        m_shadow.setFillColor(sf::Color(0, 0, 0, 90));
        m_shadow.setOrigin(m_radius * 1.1f, m_radius * 1.1f);
    }

    sf::Vector2f IsometricPhysicsItem::WorldToScreen(const sf::Vector2f& world, float height) const {
        float screenX = (world.x - world.y) * COS_ANGLE;
        float screenY = (world.x + world.y) * SIN_ANGLE - height;
        screenX += SCREEN_WIDTH / 2.0f;
        screenY += SCREEN_HEIGHT / 2.0f;
        return sf::Vector2f(screenX, screenY);
    }

    sf::Vector2f IsometricPhysicsItem::ScreenToWorld(float screenX, float screenY) const {
        float cx = screenX - SCREEN_WIDTH / 2.0f;
        float cy = screenY - SCREEN_HEIGHT / 2.0f;
        float worldX = (cx / COS_ANGLE + cy / SIN_ANGLE) / 2.0f;
        float worldZ = (cy / SIN_ANGLE - cx / COS_ANGLE) / 2.0f;
        return sf::Vector2f(worldX, worldZ);
    }

    void IsometricPhysicsItem::Spawn(float worldX, float worldZ) {
        SpawnAtWorld(worldX, worldZ);
    }

    void IsometricPhysicsItem::SpawnAtWorld(float worldX, float worldZ) {
        m_worldPos = sf::Vector2f(worldX, worldZ);
        m_height = 0.0f;

        // Случайное начальное движение
        m_velGround = sf::Vector2f((rand() % 400) - 200, (rand() % 400) - 200);
        m_velY = (rand() % 250) + 25;        // сильный начальный подскок

        sf::Vector2f screenPos = WorldToScreen(m_worldPos, m_height);
        if (m_transform) m_transform->SetPosition(screenPos);

        m_active = true;

        std::cout << "[Iso] Spawned at World(" << worldX << ", " << worldZ
            << ") velY=" << m_velY << "\n";
    }

    void IsometricPhysicsItem::SpawnInFrontOfPlayer(float distance) {
        if (!g_Application) {
            SpawnAtWorld(300.f, 200.f);
            return;
        }

        Player* player = g_Application->GetPlayer();
        if (!player) {
            SpawnAtWorld(300.f, 200.f);
            return;
        }

        auto* playerTransform = player->GetComponent<TransformComponent>();
        if (!playerTransform) {
            SpawnAtWorld(300.f, 200.f);
            return;
        }

        sf::Vector2f playerScreen = playerTransform->GetPosition();
        sf::Vector2f spawnScreen = playerScreen + sf::Vector2f(distance * 0.7f, distance * 0.5f);

        sf::Vector2f worldPos = ScreenToWorld(spawnScreen.x, spawnScreen.y);
        SpawnAtWorld(worldPos.x, worldPos.y);
    }

    void IsometricPhysicsItem::ApplyImpulse(float velX, float velZ, float velY) {
        m_velGround.x += velX;
        m_velGround.y += velZ;
        m_velY += velY;
    }
    void IsometricPhysicsItem::HandleCollisions() {
        if (!m_collision || !m_active) return;

        auto& world = GameWorld::GetInstance();
        auto& objects = world.GetAllGameObjects();

        for (auto* obj : objects) {
            if (obj == this) continue;

            auto* otherCollision = obj->GetComponent<CollisionComponent>();
            if (!otherCollision) continue;

            if (m_collision->CheckCollision(otherCollision)) {

                sf::Vector2f delta = m_transform->GetPosition() - obj->GetPosition();
                float len = std::sqrt(delta.x * delta.x + delta.y * delta.y);
                if (len < 0.01f) continue;

                delta /= len;

                float bounceStrength = 95.0f;
                float upwardBoost = 35.0f;

                // ====================== ИГРОК (СБОР ПРЕДМЕТА) ======================
                if (obj == g_Application->GetPlayer()) {
                    std::cout << "[Iso] Collected by PLAYER! +5 gold" << std::endl;

                    if (g_Application) {
                        g_Application->AddGold(5);
                    }

                    m_active = false;
                    return;
                }
                // ====================== ШАР В ШАР ======================
                else if (dynamic_cast<IsometricPhysicsItem*>(obj)) {
                    std::cout << "[Iso] Ball vs Ball" << std::endl;
                    bounceStrength = 110.0f;
                    upwardBoost = 42.0f;

                    auto* other = dynamic_cast<IsometricPhysicsItem*>(obj);
                    if (other) {
                        other->ApplyImpulse(-delta.x * 75.0f, -delta.y * 75.0f, 38.0f);
                    }
                }
                // ====================== СТЕНЫ ======================
                else {
                    std::cout << "[Iso] Weak bounce from wall/block" << std::endl;
                    bounceStrength = 95.0f;
                    upwardBoost = 35.0f;
                }

                m_velGround.x = delta.x * bounceStrength;
                m_velGround.y = delta.y * bounceStrength;
                m_velY += upwardBoost;

                float overlap = (m_radius + 22.0f) - len;
                if (overlap > 0.0f) {
                    m_worldPos += sf::Vector2f(delta.x * overlap * 0.9f, delta.y * overlap * 0.9f);
                }
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
            if (std::abs(m_velY) < 40.0f) m_velY = 0;
        }

        m_velGround *= m_damping;
        m_worldPos += m_velGround * deltaTime;

        HandleCollisions();

        const float margin = m_radius;
        if (m_worldPos.x < margin && m_velGround.x < 0) { m_velGround.x = -m_velGround.x * 0.6f; m_worldPos.x = margin; }
        if (m_worldPos.x > WORLD_WIDTH - margin && m_velGround.x > 0) { m_velGround.x = -m_velGround.x * 0.6f; m_worldPos.x = WORLD_WIDTH - margin; }
        if (m_worldPos.y < margin && m_velGround.y < 0) { m_velGround.y = -m_velGround.y * 0.6f; m_worldPos.y = margin; }
        if (m_worldPos.y > WORLD_HEIGHT - margin && m_velGround.y > 0) { m_velGround.y = -m_velGround.y * 0.6f; m_worldPos.y = WORLD_HEIGHT - margin; }

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
        if (!m_active) return;
        window.draw(m_shadow);
        window.draw(m_shape);
    }

} // namespace rogalique