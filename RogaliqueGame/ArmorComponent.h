#pragma once
#include "Component.h"
#include <iostream>

namespace rogalique
{
    class ArmorComponent : public Component
    {
    public:
        ArmorComponent(RogaliqueGameObject* owner, int maxArmor = 0, float absorptionPercent = 0.5f, float regenRate = 0.0f);
        
        void Update(float deltaTime) override;
        void Render(sf::RenderWindow& window) override;
        
        int GetCurrentArmor() const { return m_currentArmor; }
        int GetMaxArmor() const { return m_maxArmor; }
        float GetAbsorptionPercent() const { return m_absorptionPercent; }
        
        void Repair(int amount);
        void DamageArmor(int damage);
        void Reset();
        
        int CalculateDamage(int incomingDamage);
        
        bool HasArmor() const { return m_currentArmor > 0; }
        float GetArmorPercent() const { return (float)m_currentArmor / (float)m_maxArmor; }
        
        void SetAbsorptionPercent(float percent) { m_absorptionPercent = std::clamp(percent, 0.0f, 1.0f); }
        void SetRegenRate(float rate) { m_regenRate = rate; }
        
    private:
        int m_maxArmor;
        int m_currentArmor;
        float m_absorptionPercent;
        float m_regenRate;
        float m_regenTimer;
    };
}
