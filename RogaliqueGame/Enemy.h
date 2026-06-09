#pragma once
#include "RogaliqueGameObject.h"
#include <SFML/Graphics.hpp>

namespace rogalique
{
    class Enemy : public RogaliqueGameObject
    {
    public:
        Enemy();
        void Update(float deltaTime) override;
        void Render(sf::RenderWindow& window) override;

        int GetHealth() const { return m_health; }
        int GetMaxHealth() const { return m_maxHealth; }
        void TakeDamage(int damage);
        void Heal(int amount);
        
        sf::Vector2f GetPosition() const;

    private:
        void UpdateUIText();
        
        int m_health = 100;
        int m_maxHealth = 100;
        
        sf::Text m_healthText;
        sf::Font m_font;
    };
}
