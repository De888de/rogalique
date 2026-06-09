#pragma once
#include "RogaliqueGameObject.h"
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

        int GetHealth() const { return m_health; }
        void TakeDamage(int damage);
        void Heal(int amount);

        sf::Vector2f GetPosition() const;

    private:
        void Shoot();                    // ← Добавили объявление!

        int m_health = 100;
        int m_maxHealth = 100;
        bool m_hasWeapon = false;

        sf::RectangleShape m_equippedWeapon;

        float m_shootCooldown = 0.0f;
        const float m_fireRate = 0.25f;
    };
}