#include "HealthUI.h"
#include "Application.h"
#include <iostream>

namespace rogalique {

    HealthUI::HealthUI()
        : m_currentHealth(5)
        , m_maxHealth(5)
        , m_currentArmor(3)
        , m_maxArmor(3)
        , m_gameOver(false)
    {
        // Создаём 5 сердец справа сверху
        for (int i = 0; i < m_maxHealth; i++) {
            CreateHeart(SCREEN_WIDTH - 40 - (i * 35), 20);
        }
        
        // Создаём 3 щита под сердцами
        for (int i = 0; i < m_maxArmor; i++) {
            CreateShield(SCREEN_WIDTH - 40 - (i * 35), 60);
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
    
    void HealthUI::CreateShield(int x, int y) {
        sf::ConvexShape shield;
        shield.setPointCount(3);  // Треугольник
        
        // Равнобедренный треугольник (щит)
        shield.setPoint(0, sf::Vector2f(0, 0));
        shield.setPoint(1, sf::Vector2f(15, 0));
        shield.setPoint(2, sf::Vector2f(7.5f, 15));
        
        shield.setFillColor(sf::Color(255, 100, 150));  // Розовый
        shield.setPosition(x, y);
        
        m_shields.push_back(shield);
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
    
    void HealthUI::UpdateArmor(int currentArmor, int maxArmor) {
        m_currentArmor = currentArmor;
        m_maxArmor = maxArmor;
        
        // Если щитов ещё нет - создаём
        if (m_shields.empty()) {
            for (int i = 0; i < m_maxArmor; i++) {
                CreateShield(SCREEN_WIDTH - 40 - (i * 35), 60);
            }
        }
        
        for (size_t i = 0; i < m_shields.size(); i++) {
            if (i < (size_t)m_currentArmor) {
                m_shields[i].setFillColor(sf::Color(255, 100, 150));  // Розовый
            } else {
                m_shields[i].setFillColor(sf::Color(50, 50, 50, 100));  // Серый/прозрачный
            }
        }
    }
    
    void HealthUI::Render(sf::RenderWindow& window) {
        for (auto& heart : m_hearts) {
            window.draw(heart);
        }
        for (auto& shield : m_shields) {
            window.draw(shield);
        }
    }
    
    void HealthUI::Reset() {
        m_currentHealth = m_maxHealth;
        m_currentArmor = m_maxArmor;
        m_gameOver = false;
        UpdateHealth(m_currentHealth, m_maxHealth);
        UpdateArmor(m_currentArmor, m_maxArmor);
    }
}
