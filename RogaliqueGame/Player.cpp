#include <cmath>
#include "CollisionComponent.h"
#include "Player.h"
#include "Chest.h"
#include "GameWorld.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "MovementComponent.h"
#include "HealthComponent.h"
#include "Application.h"           // ← Обязательно для g_Application
#include "Bullet.h"
#include "Bullet.h"
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

namespace rogalique
{
    Player::Player()
    {
        AddComponent<TransformComponent>();
        AddComponent<SpriteComponent>("player.png", 32, 32);
        AddComponent<MovementComponent>(200.0f);
        AddComponent<HealthComponent>(100);
        AddComponent<CollisionComponent>(16.0f);

        m_equippedWeapon.setSize(sf::Vector2f(35, 12));
        m_equippedWeapon.setFillColor(sf::Color(200, 200, 50));
        m_equippedWeapon.setOrigin(5, 6);
        m_hasWeapon = true;
    }

    void Player::EquipWeapon()
    {
        if (!m_hasWeapon) {
            m_hasWeapon = true;
            std::cout << "[Player] Weapon equipped! Press F to shoot" << std::endl;
        }
    }

    void Player::Update(float deltaTime)
    {
        RogaliqueGameObject::Update(deltaTime);

        m_shootCooldown -= deltaTime;
        if (m_shootCooldown < 0.0f) m_shootCooldown = 0.0f;

        if (m_hasWeapon && sf::Keyboard::isKeyPressed(sf::Keyboard::F))
        {
            if (m_shootCooldown <= 0.0f)
            {
                Shoot();
                m_shootCooldown = m_fireRate;
            }
        }

        // Проверка сундуков
        auto* transform = GetComponent<TransformComponent>();
        if (!transform) return;

        sf::Vector2f playerPos = transform->GetPosition();

        auto& world = GameWorld::GetInstance();
        for (auto* obj : world.GetAllGameObjects())
        {
            auto* chest = dynamic_cast<Chest*>(obj);
            if (chest && !chest->IsCollected())
            {
                float dx = playerPos.x - chest->GetPosition().x;
                float dy = playerPos.y - chest->GetPosition().y;
                float dist = std::sqrt(dx * dx + dy * dy);
                if (dist < 32.0f)
                {
                    chest->Collect();
                }
            }
        }
    }

    void Player::Shoot()
    {
        std::cout << "[Player] BANG!" << std::endl;

        sf::Vector2i mouseScreen = sf::Mouse::getPosition(g_Application->window);
        sf::Vector2f mouseWorld = g_Application->window.mapPixelToCoords(mouseScreen);

        auto* transform = GetComponent<TransformComponent>();
        if (!transform) return;

        sf::Vector2f playerPos = transform->GetPosition();
        sf::Vector2f dir = mouseWorld - playerPos;

        float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        if (len > 0.0f)
            dir /= len;

        // Создаём пулю
        Bullet* bullet = new Bullet(playerPos, dir, 600.0f);

        // Добавляем в GameWorld
        auto& world = GameWorld::GetInstance();
        world.AddGameObject(bullet);

        std::cout << "[Shoot] Direction: (" << dir.x << ", " << dir.y << ")" << std::endl;
    }

    void Player::Render(sf::RenderWindow& window)
    {
        RogaliqueGameObject::Render(window);

        if (m_hasWeapon)
        {
            auto* transform = GetComponent<TransformComponent>();
            if (transform)
            {
                sf::Vector2f playerPos = transform->GetPosition();

                sf::Vector2i mouseScreen = sf::Mouse::getPosition(g_Application->window);
                sf::Vector2f mouseWorld = g_Application->window.mapPixelToCoords(mouseScreen);

                sf::Vector2f dir = mouseWorld - playerPos;
                float angle = std::atan2(dir.y, dir.x) * 180.0f / 3.14159265f;

                m_equippedWeapon.setRotation(angle);
                m_equippedWeapon.setPosition(playerPos.x + 18.f, playerPos.y - 5.f);
               

                window.draw(m_equippedWeapon);
            }
        }
    }

    sf::Vector2f Player::GetPosition() const
    {
        auto* transform = GetComponent<TransformComponent>();
        if (transform)
            return transform->GetPosition();
        return sf::Vector2f(0, 0);
    }
}