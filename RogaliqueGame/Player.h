#pragma once
#include "RogaliqueGameObject.h"

namespace rogalique
{
    class Player : public RogaliqueGameObject
    {
    public:
        Player();
        void Update(float deltaTime) override;
        
        int GetHealth() const { return m_health; }
        void TakeDamage(int damage);
        void Heal(int amount);
        
    private:
        int m_health = 100;
        int m_maxHealth = 100;
    };
}
