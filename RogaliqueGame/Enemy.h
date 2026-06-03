#pragma once
#include "RogaliqueGameObject.h"

namespace rogalique
{
    class Enemy : public RogaliqueGameObject
    {
    public:
        Enemy();
        void Update(float deltaTime) override;

        int GetHealth() const { return m_health; }
        void TakeDamage(int damage);
        void Heal(int amount);

    private:
        int m_health = 30;
        int m_maxHealth = 30;
    };
}
