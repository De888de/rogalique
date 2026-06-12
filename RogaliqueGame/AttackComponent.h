#pragma once
#include "Component.h"
#include <SFML/Graphics.hpp>

namespace rogalique
{
    class Player;
    
    class AttackComponent : public Component
    {
    public:
        AttackComponent(RogaliqueGameObject* owner, int damage = 1, float range = 40.0f, float delay = 1.0f);
        
        void Update(float deltaTime) override;
        void Render(sf::RenderWindow& window) override;
        
        bool CanAttack() const;
        bool IsInRange(Player* target) const;
        void Attack(Player* target);
        
        // Сеттеры
        void SetDamage(int damage) { m_attackDamage = damage; }
        void SetRange(float range) { m_attackRange = range; }
        void SetDelay(float delay) { m_attackDelay = delay; }
        
        // Геттеры
        float GetCooldownPercent() const { return m_attackCooldown / m_attackDelay; }
        bool IsOnCooldown() const { return m_attackCooldown > 0.0f; }
        
    private:
        void PlayRandomAttackSound();  // ← ДОБАВИТЬ ЭТУ СТРОКУ
        
        float m_attackRange = 40.0f;
        float m_attackCooldown = 0.0f;
        float m_attackDelay = 1.0f;
        int m_attackDamage = 1;
    };
}
