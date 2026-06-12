#include <cmath>
#include <cassert>
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
#include "ArmorComponent.h"
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
        AddComponent<HealthComponent>(5);  // 5 жизней
        AddComponent<ArmorComponent>(5, 0.5f, 0.5f);  // 5 брони, 50% поглощения, 0.5 реген/сек
        AddComponent<CollisionComponent>(16.0f);


        m_equippedWeapon.setSize(sf::Vector2f(35, 12));
        m_equippedWeapon.setFillColor(sf::Color(200, 200, 50));
        m_equippedWeapon.setOrigin(5, 6);

        auto* armor = GetComponent<ArmorComponent>();
        if (armor) {
            std::cout << "[Player] Armor equipped: " << armor->GetCurrentArmor() << "/" << armor->GetMaxArmor() << std::endl;
        }
        else {
            std::cout << "[Player] NO ARMOR COMPONENT!" << std::endl;
        }
    }

    void Player::SetWeapon(Weapon* weapon)
    {
        if (m_weapon) {
            delete m_weapon;
        }
        m_weapon = weapon;
        m_hasWeapon = true;
        LOG_EVENT("Weapon Equipped", weapon->GetName());
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

        // Обновление неуязвимости
        if (m_invulnerableTimer > 0.0f) {
            m_invulnerableTimer -= deltaTime;
        }

        // Обновление оружия
        if (m_weapon) {
            m_weapon->Update(deltaTime);
        }

        // Стрельба
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

        // Перезарядка
        static bool rPressed = false;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
            if (!rPressed && m_weapon) {
                m_weapon->Reload();
                if (g_Application) {
                    g_Application->UpdateUI();
                }
                rPressed = true;
            }
        }
        else {
            rPressed = false;
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

            // 25% урон от максимального здоровья врага
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
        assert(transform && "Player must have TransformComponent");
        return transform ? transform->GetPosition() : sf::Vector2f(0, 0);
    }

    // ========== HealthComponent методы ==========

    int Player::GetHealth() const
    {
        auto* health = GetComponent<HealthComponent>();
        return health ? health->GetHealth() : 0;
    }

    int Player::GetMaxHealth() const
    {
        auto* health = GetComponent<HealthComponent>();
        return health ? health->GetMaxHealth() : 0;
    }

    bool Player::IsAlive() const
    {
        auto* health = GetComponent<HealthComponent>();
        return health && health->IsAlive();
    }

    void Player::TakeDamage(int damage)
    {
        assert(damage > 0 && "Damage must be positive");

        // Проверка на неуязвимость
        if (IsInvulnerable()) {
            std::cout << "[Player] Invulnerable, no damage" << std::endl;
            return;
        }

        auto* health = GetComponent<HealthComponent>();
        assert(health && "Player must have HealthComponent");
        if (!health) return;

        // Расчёт урона с учётом брони
        auto* armor = GetComponent<ArmorComponent>();
        int finalDamage = damage;

        if (armor && armor->HasArmor()) {
            finalDamage = armor->CalculateDamage(damage);
            std::cout << "[Player] Armor absorbed part of damage. Original: " << damage
                << ", Final: " << finalDamage << std::endl;
        }
        else {
            SoundManager::GetInstance().PlaySound("hit");
        }

        int oldHealth = health->GetHealth();
        LOG_PLAYER_DAMAGE(oldHealth);

        health->TakeDamage(finalDamage);
        m_invulnerableTimer = m_invulnerableDuration;

        int newHealth = health->GetHealth();
        std::cout << "[Player] Hit! -" << finalDamage << " HP, Health: " << newHealth << "/" << GetMaxHealth() << std::endl;

        // Проверка на смерть
        if (!IsAlive())
        {
            LOG_PLAYER_DEATH();
            std::cout << "[Player] DIED! Game Over!" << std::endl;
            if (g_Application) {
                g_Application->ShowGameOver();
            }
        }
        // Обновляем UI здоровья и брони
        if (g_Application) {
            g_Application->UpdateHealthUI(newHealth, GetMaxHealth());
            auto* armor = GetComponent<ArmorComponent>();
            if (armor) {
                g_Application->UpdateArmorUI(armor->GetCurrentArmor(), armor->GetMaxArmor());
            }
        }
    }

    void Player::Heal(int amount)
    {
        auto* health = GetComponent<HealthComponent>();
        if (!health) return;

        int oldHealth = health->GetHealth();
        health->Heal(amount);
        int newHealth = health->GetHealth();

        std::cout << "[Player] Healed +" << (newHealth - oldHealth) << " HP, Health: " << newHealth << "/" << GetMaxHealth() << std::endl;

        if (g_Application) {
            g_Application->UpdateHealthUI(newHealth, GetMaxHealth());
        }
    }
}