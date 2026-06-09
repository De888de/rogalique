#include "HealthUI.h"
#include "Application.h"
#include <iostream>

namespace rogalique {

    HealthUI::HealthUI()
        : m_currentHealth(5)
        , m_maxHealth(5)
        , m_gameOver(false)
    {
        // Создаём 5 сердец справа сверху
        for (int i = 0; i < m_maxHealth; i++) {
            CreateHeart(SCREEN_WIDTH - 40 - (i * 35), 20);
        }
    }
    
    void HealthUI::CreateHeart(int x, int y) {
        sf::ConvexShape heart;
        heart.setPointCount(6);
        
        heart.setPoint(0, sf::Vector2f(0, 5));
        heart.setPoint(1, sf::Vector2f(5, 0));
        heart.setPoint(2, sf::Vector2f(10, 5));
        heart.setPoint(3, sf::Vector2f(10, 12));
        heart.setPoint(4, sf::Vector2f(5, 18));
        heart.setPoint(5, sf::Vector2f(0, 12));
        
        heart.setFillColor(sf::Color::Red);
        heart.setPosition(x, y);
        
        m_hearts.push_back(heart);
    }
    
    void HealthUI::UpdateHealth(int currentHealth, int maxHealth) {
        m_currentHealth = currentHealth;
        m_maxHealth = maxHealth;
        
        for (size_t i = 0; i < m_hearts.size(); i++) {
            if (i < (size_t)m_currentHealth) {
                m_hearts[i].setFillColor(sf::Color::Red);
            } else {
                m_hearts[i].setFillColor(sf::Color(50, 50, 50, 100));
            }
        }
        
        if (m_currentHealth <= 0) {
            m_gameOver = true;
            std::cout << "[HealthUI] GAME OVER!" << std::endl;
        }
    }
    
    void HealthUI::Render(sf::RenderWindow& window) {
        for (auto& heart : m_hearts) {
            window.draw(heart);
        }
    }
    
    void HealthUI::Reset() {
        m_currentHealth = m_maxHealth;
        m_gameOver = false;
        UpdateHealth(m_currentHealth, m_maxHealth);
    }
}
