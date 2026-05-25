#pragma once
#include "Component.h"

namespace rogalique
{
    class HealthComponent : public Component
    {
    public:
        HealthComponent(RogaliqueGameObject* owner, int maxHealth);
        void Update(float deltaTime) override;
        void Render(sf::RenderWindow& window) override;
        
        int GetHealth() const { return m_health; }
        void TakeDamage(int damage);
        void Heal(int amount);
        
    private:
        int m_health;
        int m_maxHealth;
    };
}
