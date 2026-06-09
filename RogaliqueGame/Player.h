#pragma once
#include "RogaliqueGameObject.h"
#include "Weapon.h"
#include <SFML/Graphics.hpp>

namespace rogalique
{
    class Player : public RogaliqueGameObject
    {
    public:
        Player();
        void Update(float deltaTime) override;
        void Render(sf::RenderWindow& window) override;

        bool HasWeapon() const { return m_hasWeapon; }
        void EquipWeapon();
        
        Weapon* GetWeapon() const { return m_weapon; }
        void SetWeapon(Weapon* weapon);

        int GetHealth() const { return m_health; }
        int GetMaxHealth() const { return m_maxHealth; }
        void TakeDamage(int damage);
        void Heal(int amount);
        bool IsInvulnerable() const { return m_invulnerableTimer > 0.0f; }

        sf::Vector2f GetPosition() const;

    private:
        void Shoot();

        int m_health = 5;
        int m_maxHealth = 5;
        bool m_hasWeapon = false;

        sf::RectangleShape m_equippedWeapon;
        
        Weapon* m_weapon = nullptr;

        float m_shootCooldown = 0.0f;
        const float m_fireRate = 0.25f;
        
        // Система неуязвимости и мигания
        float m_invulnerableTimer = 0.0f;
        float m_invulnerableDuration = 1.0f;
    };
}
