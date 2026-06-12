#pragma once
#include "RogaliqueGameObject.h"
#include "HealthComponent.h"
#include <SFML/Graphics.hpp>

namespace rogalique
{
    class Enemy : public RogaliqueGameObject
    {
    public:
        Enemy();
        void Update(float deltaTime) override;
        void Render(sf::RenderWindow& window) override;

        int GetHealth() const;
        int GetMaxHealth() const;
        void TakeDamage(int damage);
        void Heal(int amount);
        bool IsAlive() const;
        
        sf::Vector2f GetPosition() const;

    private:
        void UpdateUIText();
        
        HealthComponent* m_healthComponent = nullptr;
        sf::Text m_healthText;
        sf::Font m_font;
    };
}
