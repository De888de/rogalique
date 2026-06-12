#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

namespace rogalique {

    class HealthUI {
    public:
        HealthUI();
        void UpdateHealth(int currentHealth, int maxHealth);
        void UpdateArmor(int currentArmor, int maxArmor);
        void Render(sf::RenderWindow& window);
        bool IsGameOver() const { return m_gameOver; }
        void Reset();
        
    private:
        void CreateHeart(int x, int y);
        void CreateShield(int x, int y);
        
        std::vector<sf::ConvexShape> m_hearts;
        std::vector<sf::ConvexShape> m_shields;
        int m_currentHealth;
        int m_maxHealth;
        int m_currentArmor;
        int m_maxArmor;
        bool m_gameOver;
    };
    
}
