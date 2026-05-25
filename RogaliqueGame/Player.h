#pragma once
#include "RogaliqueGameObject.h"

namespace rogalique
{
    class Player : public RogaliqueGameObject
    {
    public:
        Player();
        void Update(float deltaTime) override;
        void Draw(sf::RenderWindow& window) override;
        
        int GetHealth() const { return m_health; }
        void TakeDamage(int damage);
        void Heal(int amount);
        
    private:
        sf::RectangleShape m_shape;
        int m_health = 100;
        int m_maxHealth = 100;
        float m_speed = 200.0f;
    };
}
