#include "AttackComponent.h"
#include "Player.h"
#include "TransformComponent.h"
#include "SoundManager.h"
#include "Application.h"
#include <iostream>
#include <cassert>
#include <cmath>
#include <random>
#include <cstdlib>
#include <ctime>

namespace rogalique
{
    AttackComponent::AttackComponent(RogaliqueGameObject* owner, int damage, float range, float delay)
        : Component(owner)
        , m_attackDamage(damage)
        , m_attackRange(range)
        , m_attackDelay(delay)
        , m_attackCooldown(0.0f)
    {
        std::cout << "[AttackComponent] Created with damage=" << damage 
                  << ", range=" << range << ", delay=" << delay << std::endl;
    }
    
    void AttackComponent::Update(float deltaTime)
    {
        if (m_attackCooldown > 0.0f) {
            m_attackCooldown -= deltaTime;
            if (m_attackCooldown < 0.0f) {
                m_attackCooldown = 0.0f;
            }
        }
    }
    
    void AttackComponent::Render(sf::RenderWindow& window)
    {
        // Опционально: визуализация радиуса атаки для дебага
    }
    
    bool AttackComponent::CanAttack() const
    {
        bool result = m_attackCooldown <= 0.0f;
        std::cout << "[AttackComponent] CanAttack: cooldown=" << m_attackCooldown
            << ", result=" << result << std::endl;
        return result;
    }
    
    bool AttackComponent::IsInRange(Player* target) const
    {
        if (!target || !GetOwner()) {
            std::cout << "[AttackComponent] IsInRange: no target or owner" << std::endl;
            return false;
        }

        auto* ownerTransform = GetOwner()->GetComponent<TransformComponent>();
        auto* targetTransform = target->GetComponent<TransformComponent>();

        if (!ownerTransform || !targetTransform) {
            std::cout << "[AttackComponent] IsInRange: missing transform components" << std::endl;
            return false;
        }

        sf::Vector2f ownerPos = ownerTransform->GetPosition();
        sf::Vector2f targetPos = targetTransform->GetPosition();

        float dx = ownerPos.x - targetPos.x;
        float dy = ownerPos.y - targetPos.y;
        float dist = std::sqrt(dx * dx + dy * dy);

        bool result = dist <= m_attackRange;
        std::cout << "[AttackComponent] IsInRange: dist=" << dist
            << ", range=" << m_attackRange
            << ", result=" << result << std::endl;

        return result;
    }
    
    void AttackComponent::PlayRandomAttackSound()
    {
        static bool seeded = false;
        if (!seeded) {
            std::srand(static_cast<unsigned>(std::time(nullptr)));
            seeded = true;
        }
        
        int randomSound = std::rand() % 2;
        
        if (randomSound == 0) {
            SoundManager::GetInstance().PlaySound("attack_01");
        } else {
            SoundManager::GetInstance().PlaySound("attack_02");
        }
    }
    
    void AttackComponent::Attack(Player* target)
    {
        if (!CanAttack()) {
            return;
        }
        
        if (!IsInRange(target)) {
            return;
        }
        
        if (!target || !target->IsAlive()) {
            return;
        }
        
        target->TakeDamage(m_attackDamage);
        m_attackCooldown = m_attackDelay;
        PlayRandomAttackSound();
        
        std::cout << "[AttackComponent] Attack! Damage=" << m_attackDamage 
                  << ", Target HP=" << target->GetHealth() << std::endl;
    }
}
