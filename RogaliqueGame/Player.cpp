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
        AddComponent<HealthComponent>(5);
        AddComponent<ArmorComponent>(5, 0.5f, 0.5f);
        AddComponent<CollisionComponent>(16.0f);

        auto* armor = GetComponent<ArmorComponent>();
        if (armor) {
            std::cout << "[Player] Armor equipped: " << armor->GetCurrentArmor() << "/" << armor->GetMaxArmor() << std::endl;
        }
    }

    void Player::SetWeapon(Weapon* weapon)
    {
        if (m_weapon) {
            delete m_weapon;
        }
        m_weapon = weapon;
        LOG_EVENT("Weapon Equipped", weapon->GetName());
        std::cout << "[Player] Equipped: " << m_weapon->GetName() << std::endl;
    }

    void Player::EquipWeapon()
    {
        if (!m_weapon) {
            Weapon* starterWeapon = new Weapon("Iron Pistol", 20, 0.25f, 10, 600.0f, 32.0f);
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

        // Обновление оружия (кулдаун)
        if (m_weapon) {
            m_weapon->Update(deltaTime);
        }

        // Стрельба
        static bool fPressed = false;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
            if (!fPressed && HasWeapon()) {
                Shoot();
                fPressed = true;
            }
        }
        else {
            fPressed = false;
        }

        // Перезарядка
        static bool rPressed = false;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
            if (!rPressed && m_weapon) {
                m_weapon->Reload();
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

        // Получаем позицию игрока
        auto* transform = GetComponent<TransformComponent>();
        if (!transform) return;
        sf::Vector2f playerPos = transform->GetPosition();

        // Получаем позицию курсора
        sf::Vector2i mouseScreen = sf::Mouse::getPosition(g_Application->window);
        sf::Vector2f mouseWorld = g_Application->window.mapPixelToCoords(mouseScreen);

        // Оружие делает всё
        Bullet* bullet = m_weapon->Shoot(playerPos, mouseWorld);

        if (bullet) {
            SoundManager::GetInstance().PlaySound("shot");
            GameWorld::GetInstance().AddGameObject(bullet);
        }
    }

    void Player::Render(sf::RenderWindow& window)
    {
        RogaliqueGameObject::Render(window);

        // Оружие рисует само себя
        if (m_weapon && HasWeapon())
        {
            auto* transform = GetComponent<TransformComponent>();
            if (transform)
            {
                sf::Vector2f playerPos = transform->GetPosition();

                sf::Vector2i mouseScreen = sf::Mouse::getPosition(g_Application->window);
                sf::Vector2f mouseWorld = g_Application->window.mapPixelToCoords(mouseScreen);

                sf::Vector2f dir = mouseWorld - playerPos;
                float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
                if (len > 0.0f) dir /= len;

                m_weapon->Render(window, playerPos, dir);
            }
        }
    }

    sf::Vector2f Player::GetPosition() const
    {
        auto* transform = GetComponent<TransformComponent>();
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

        if (IsInvulnerable()) {
            std::cout << "[Player] Invulnerable, no damage" << std::endl;
            return;
        }

        auto* health = GetComponent<HealthComponent>();
        auto* armor = GetComponent<ArmorComponent>();

        if (!health) return;

        int finalDamage = damage;
        if (armor) {
            finalDamage = armor->CalculateDamage(damage);
        }

        if (finalDamage <= 0) {
            if (g_Application && armor) {
                g_Application->UpdateArmorUI(armor->GetCurrentArmor(), armor->GetMaxArmor());
            }
            return;
        }

        int oldHealth = health->GetHealth();

        health->TakeDamage(finalDamage);
        m_invulnerableTimer = m_invulnerableDuration;

        int newHealth = health->GetHealth();

        // Звук
        if (newHealth <= 3) {
            SoundManager::GetInstance().PlaySound("death_scream");
        }
        else {
            SoundManager::GetInstance().PlaySound("hit");
        }

        LOG_PLAYER_DAMAGE(newHealth);
        std::cout << "[Player] Hit! -" << finalDamage << " HP, Health: " << newHealth << "/" << GetMaxHealth() << std::endl;

        if (!IsAlive())
        {
            LOG_PLAYER_DEATH();
            std::cout << "[Player] DIED! Game Over!" << std::endl;
            if (g_Application) {
                g_Application->ShowGameOver();
            }
        }

        if (g_Application) {
            g_Application->UpdateHealthUI(newHealth, GetMaxHealth());
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