#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

namespace rogalique {

    class HealthUI {
    public:
        HealthUI();
        void UpdateHealth(int currentHealth, int maxHealth);
        void Render(sf::RenderWindow& window);
        bool IsGameOver() const { return m_gameOver; }
        void Reset();
        
    private:
        void CreateHeart(int x, int y);
        
        std::vector<sf::ConvexShape> m_hearts;
        int m_currentHealth;
        int m_maxHealth;
        bool m_gameOver;
    };
    
}
