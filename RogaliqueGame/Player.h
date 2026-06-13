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

        bool HasWeapon() const { return m_weapon != nullptr; }
        void EquipWeapon();
        
        Weapon* GetWeapon() const { return m_weapon; }
        void SetWeapon(Weapon* weapon);

        int GetHealth() const;
        int GetMaxHealth() const;
        void TakeDamage(int damage);
        void Heal(int amount);
        bool IsAlive() const;
        
        bool IsInvulnerable() const { return m_invulnerableTimer > 0.0f; }
        sf::Vector2f GetPosition() const;

    private:
        void Shoot();

        Weapon* m_weapon = nullptr;
        
        // Система неуязвимости
        float m_invulnerableTimer = 0.0f;
        float m_invulnerableDuration = 1.0f;
    };
}
