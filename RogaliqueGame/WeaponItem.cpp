#include "WeaponItem.h"

namespace rogalique {

    WeaponItem::WeaponItem(float x, float y)
        : m_position(x, y)
        , m_isOnGround(true)
        , m_pickupRadius(50.0f)
        , m_floatTime(0.0f)
    {
        m_weaponShape.setSize(sf::Vector2f(30, 20));
        m_weaponShape.setFillColor(sf::Color(148, 0, 211));
        m_weaponShape.setOrigin(15, 10);
        m_weaponShape.setPosition(x, y);

        std::cout << "[WeaponItem] Spawned at (" << x << ", " << y << ")" << std::endl;
    }

    void WeaponItem::Update(float dt)
    {
        if (!m_isOnGround) return;

        m_floatTime += dt;
        float offsetY = std::sin(m_floatTime * 3.0f) * 5.0f;
        m_weaponShape.setPosition(m_position.x, m_position.y + offsetY);
    }

    void WeaponItem::Render(sf::RenderWindow& window)
    {
        if (m_isOnGround) {
            window.draw(m_weaponShape);
        }
    }

    bool WeaponItem::CheckPickup(sf::Vector2f playerPos)
    {
        if (!m_isOnGround) return false;

        float dx = playerPos.x - m_position.x;
        float dy = playerPos.y - m_position.y;
        float dist = std::sqrt(dx * dx + dy * dy);

        if (dist < m_pickupRadius) {
            m_isOnGround = false;
            std::cout << "[WeaponItem] Picked up by player!" << std::endl;
            return true;
        }
        return false;
    }

}