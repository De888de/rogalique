#include "ArmorComponent.h"
#include "SoundManager.h"
#include "Application.h"
#include <algorithm>
#include <cassert>
#include <iostream>

namespace rogalique
{
    ArmorComponent::ArmorComponent(RogaliqueGameObject* owner, int maxArmor, float absorptionPercent, float regenRate)
        : Component(owner)
        , m_maxArmor(maxArmor)
        , m_currentArmor(maxArmor)
        , m_absorptionPercent(std::clamp(absorptionPercent, 0.0f, 1.0f))
        , m_regenRate(regenRate)
        , m_regenTimer(0.0f)
    {
        assert(maxArmor >= 0 && "Max armor cannot be negative");
        std::cout << "[ArmorComponent] Created with " << m_maxArmor 
                  << " armor, absorption: " << (m_absorptionPercent * 100) << "%, regen: " << regenRate << "/s" << std::endl;
    }
    
    void ArmorComponent::Update(float deltaTime)
    {
        // Регенерация брони
        if (m_regenRate > 0.0f && m_currentArmor < m_maxArmor)
        {
            m_regenTimer += deltaTime;
            if (m_regenTimer >= 1.0f)
            {
                int regenAmount = static_cast<int>(m_regenRate);
                if (regenAmount > 0) {
                    Repair(regenAmount);
                }
                m_regenTimer = 0.0f;
            }
        }
    }
    
    void ArmorComponent::Render(sf::RenderWindow& window)
    {
        (void)window;
    }
    
    void ArmorComponent::Repair(int amount)
    {
        assert(amount > 0 && "Repair amount must be positive");
        int oldArmor = m_currentArmor;
        m_currentArmor = std::min(m_maxArmor, m_currentArmor + amount);
        std::cout << "[ArmorComponent] Repaired +" << (m_currentArmor - oldArmor) 
                  << ", current: " << m_currentArmor << "/" << m_maxArmor << std::endl;
    }
    
    void ArmorComponent::DamageArmor(int damage)
    {
        assert(damage > 0 && "Damage must be positive");
        m_currentArmor = std::max(0, m_currentArmor - damage);
        std::cout << "[ArmorComponent] Armor damaged -" << damage 
                  << ", current: " << m_currentArmor << "/" << m_maxArmor << std::endl;
    }
    
    int ArmorComponent::CalculateDamage(int incomingDamage)
    {
        assert(incomingDamage > 0 && "Incoming damage must be positive");
        
        int finalDamage = incomingDamage;
        
        if (m_currentArmor > 0)
        {
            int absorbedDamage = static_cast<int>(incomingDamage * m_absorptionPercent);
            absorbedDamage = std::min(absorbedDamage, m_currentArmor);
            
            if (absorbedDamage > 0)
            {
                m_currentArmor -= absorbedDamage;
                finalDamage = incomingDamage - absorbedDamage;
                
                SoundManager::GetInstance().PlaySound("metal_hit");
                
                std::cout << "[ArmorComponent] Damage: " << incomingDamage 
                          << ", absorbed: " << absorbedDamage 
                          << ", final: " << finalDamage 
                          << ", armor left: " << m_currentArmor << std::endl;
                
                if (m_currentArmor <= 0) {
                    SoundManager::GetInstance().PlaySound("hit");
                }
            }
            else
            {
                SoundManager::GetInstance().PlaySound("hit");
            }
        }
        else
        {
            SoundManager::GetInstance().PlaySound("hit");
            std::cout << "[ArmorComponent] No armor, full damage: " << finalDamage << std::endl;
        }
        
        return std::max(1, finalDamage);
    }
    
    void ArmorComponent::Reset()
    {
        m_currentArmor = m_maxArmor;
        m_regenTimer = 0.0f;
        std::cout << "[ArmorComponent] Reset to full armor: " << m_currentArmor << "/" << m_maxArmor << std::endl;
    }
}
