#include "IsometricPhysicsItem.h"
#include "TransformComponent.h"
#include "Application.h"
#include <iostream>
#include <cmath>

namespace rogalique {

// Константы для изометрической проекции
static const float ISO_ANGLE = 0.523599f; // 30 градусов в радианах
static const float COS_ANGLE = std::cos(ISO_ANGLE);
static const float SIN_ANGLE = std::sin(ISO_ANGLE);

IsometricPhysicsItem::IsometricPhysicsItem() {
    auto transform = AddComponent<TransformComponent>();
    if (transform) {
        transform->SetPosition(sf::Vector2f(0, 0));
    }
    
    // Настройка внешнего вида
    m_shape.setRadius(m_radius);
    m_shape.setFillColor(sf::Color::Red);
    m_shape.setOrigin(m_radius, m_radius);
    
    m_shadow.setRadius(m_radius);
    m_shadow.setFillColor(sf::Color(0, 0, 0, 100)); // полупрозрачная тень
    m_shadow.setOrigin(m_radius, m_radius);
}

sf::Vector2f IsometricPhysicsItem::WorldToScreen(const sf::Vector2f& world, float height) const {
    // Изометрическая проекция
    float screenX = (world.x - world.y) * COS_ANGLE;
    float screenY = (world.x + world.y) * SIN_ANGLE - height;
    
    // Центрирование на экране (центр мира в центре экрана)
    screenX += SCREEN_WIDTH / 2.0f;
    screenY += SCREEN_HEIGHT / 2.0f;
    
    return sf::Vector2f(screenX, screenY);
}

void IsometricPhysicsItem::Spawn(float worldX, float worldZ) {
    m_worldPos = sf::Vector2f(worldX, worldZ);
    m_height = 0.0f;
    m_velGround = sf::Vector2f(0, 0);
    m_velY = 0.0f;
    
    // Случайная начальная скорость
    m_velGround.x = (rand() % 400) - 200;
    m_velGround.y = (rand() % 400) - 200;
    m_velY = (rand() % 500) + 200; // подбрасываем вверх
    
    // Обновляем TransformComponent для камеры
    auto transform = GetComponent<TransformComponent>();
    if (transform) {
        transform->SetPosition(WorldToScreen(m_worldPos, m_height));
    }
    
    m_active = true;
    std::cout << "Isometric item spawned at world (" << worldX << ", " << worldZ 
              << ") height: " << m_height << std::endl;
}

void IsometricPhysicsItem::ApplyImpulse(float velX, float velZ, float velY) {
    m_velGround.x += velX;
    m_velGround.y += velZ;
    m_velY += velY;
}

void IsometricPhysicsItem::Update(float deltaTime) {
    if (!m_active) return;
    
    // 1. Вертикальная физика (высота)
    m_velY -= m_gravity * deltaTime;  // гравитация тянет вниз
    m_height += m_velY * deltaTime;
    
    // Отскок от земли
    if (m_height < 0) {
        m_height = 0;
        m_velY = -m_velY * m_bounciness;
        
        // Если отскок слишком маленький — останавливаем
        if (std::abs(m_velY) < 50.0f) {
            m_velY = 0;
        }
        
        if (m_height == 0 && m_velY == 0) {
            std::cout << "Item landed!" << std::endl;
        }
    }
    
    // 2. Горизонтальная физика (движение по земле)
    m_velGround *= m_damping;
    m_worldPos += m_velGround * deltaTime;
    
    // 3. Границы мира
    float radius = m_radius;
    float left = radius;
    float right = WORLD_WIDTH - radius;
    float top = radius;
    float bottom = WORLD_HEIGHT - radius;
    
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
    
    // 4. Преобразование в экранные координаты
    sf::Vector2f screenPos = WorldToScreen(m_worldPos, m_height);
    
    // Обновляем TransformComponent (для камеры)
    auto transform = GetComponent<TransformComponent>();
    if (transform) {
        transform->SetPosition(screenPos);
    }
    
    // Обновляем спрайты
    m_shape.setPosition(screenPos);
    m_shadow.setPosition(WorldToScreen(m_worldPos, 0)); // тень на земле
    
    // Удаление за границами мира
    if (m_worldPos.x < -500 || m_worldPos.x > WORLD_WIDTH + 500 ||
        m_worldPos.y < -500 || m_worldPos.y > WORLD_HEIGHT + 500) {
        m_active = false;
        std::cout << "Item removed (out of bounds)" << std::endl;
    }
}

void IsometricPhysicsItem::Render(sf::RenderWindow& window) {
    if (m_active) {
        // Рисуем тень сначала (под предметом)
        window.draw(m_shadow);
        // Потом сам предмет
        window.draw(m_shape);
    }
}

} // namespace rogalique
