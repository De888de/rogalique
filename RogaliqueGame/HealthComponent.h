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
        int GetMaxHealth() const { return m_maxHealth; }
        void TakeDamage(int damage);
        void Heal(int amount);
        bool IsAlive() const { return m_health > 0; }
        
    private:

        int m_health;
        int m_maxHealth;
    };
}
