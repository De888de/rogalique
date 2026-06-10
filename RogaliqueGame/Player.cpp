#include <cmath>
#include "Logger.h"
#include "CollisionComponent.h"
#include "Player.h"
#include "Chest.h"
#include "GameWorld.h"
#include "TransformComponent.h"
#include "SoundManager.h"
#include "SpriteComponent.h"
#include "MovementComponent.h"
#include "HealthComponent.h"
#include "Application.h"
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
    }

    void Player::SetWeapon(Weapon* weapon)
    {
        if (m_weapon) {
            delete m_weapon;
        }
        m_weapon = weapon;
        m_hasWeapon = true;
        std::cout << "[Player] Equipped: " << m_weapon->GetName() << std::endl;
    }

    void Player::EquipWeapon()
    {
        if (!m_hasWeapon) {
            Weapon* starterWeapon = new Weapon("Iron Pistol", 20, 0.25f, 10);
            SetWeapon(starterWeapon);
        }
    }

   

    void Player::Update(float deltaTime)
    {
        RogaliqueGameObject::Update(deltaTime);

        if (m_invulnerableTimer > 0.0f) {
            m_invulnerableTimer -= deltaTime;
        }
        
        if (m_weapon) {
            m_weapon->Update(deltaTime);
        }

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


        
        static bool rPressed = false;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
            if (!rPressed && m_weapon) {
                m_weapon->Reload();
                if (g_Application) {
                    g_Application->UpdateUI();
                }
                rPressed = true;
            }
        } else {
            rPressed = false;
        }

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
        if (!m_weapon) {
            std::cout << "[Player] No weapon!" << std::endl;
            return;
        }
        
        if (!m_weapon->CanShoot()) {
            return;
        }
        
        sf::Vector2i mouseScreen = sf::Mouse::getPosition(g_Application->window);
        sf::Vector2f mouseWorld = g_Application->window.mapPixelToCoords(mouseScreen);
        
        auto* transform = GetComponent<TransformComponent>();
        if (!transform) return;
        
        sf::Vector2f playerPos = transform->GetPosition();
        sf::Vector2f dir = mouseWorld - playerPos;
        float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        if (len > 0.0f) dir /= len;
        
        if (m_weapon->Shoot()) {

            SoundManager::GetInstance().PlaySound("shot");

            float damageMultiplier = 0.25f;
            Bullet* bullet = new Bullet(playerPos, dir, 600.0f);
            auto& world = GameWorld::GetInstance();
            world.AddGameObject(bullet);
            
            if (g_Application) {
                g_Application->UpdateUI();
            }
        }
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
    void Player::TakeDamage(int damage)
    {
        LOG_PLAYER_DAMAGE(m_health);
        if (m_health <= 0) {
            LOG_PLAYER_DEATH();
        }

        if (IsInvulnerable()) return;
        if (m_health <= 0) return;

        if (g_Application) {
            SoundManager::GetInstance().PlaySound("hit");
        }

        m_health -= damage;
        m_invulnerableTimer = m_invulnerableDuration;

        std::cout << "[Player] Hit! -" << damage << " HP, Health: " << m_health << "/" << m_maxHealth << std::endl;

        if (m_health <= 0)
        {
            m_health = 0;
            std::cout << "[Player] DIED! Game Over!" << std::endl;

            // Вызываем Game Over через Application
            if (g_Application)
            {
                g_Application->ShowGameOver();
            }
        }

        // Обновляем UI
        if (g_Application)
        {
            g_Application->UpdateHealthUI(m_health, m_maxHealth);
        }
    }

    void Player::Heal(int amount)
    {
        m_health += amount;
        if (m_health > m_maxHealth) m_health = m_maxHealth;
        std::cout << "[Player] Healed +" << amount << " HP, Health: " << m_health << "/" << m_maxHealth << std::endl;
    }
}
