#include "UIComponent.h"
#include <iostream>

namespace rogalique
{
    UIComponent::UIComponent(RogaliqueGameObject* owner)
        : Component(owner)
    {
        if (!m_font.loadFromFile("D:/xyz/roqalique/RogaliqueGame/Resources/Fonts/Roboto-Regular.ttf"))
        {
            std::cout << "[UIComponent] Warning: Could not load font" << std::endl;
        }
        
        m_goldText.setFont(m_font);
        m_goldText.setCharacterSize(24);
        m_goldText.setFillColor(sf::Color::Yellow);
        m_goldText.setPosition(20, 20);
        
        m_chestText.setFont(m_font);
        m_chestText.setCharacterSize(24);
        m_chestText.setFillColor(sf::Color::Yellow);
        m_chestText.setPosition(20, 50);
        
        UpdateGold(0);
        UpdateChests(0);
    }
    
    void UIComponent::Update(float deltaTime)
    {
        (void)deltaTime;
    }
    
    void UIComponent::Render(sf::RenderWindow& window)
    {
        window.draw(m_goldText);
        window.draw(m_chestText);
    }
    
    void UIComponent::UpdateGold(int gold)
    {
        m_gold = gold;
        m_goldText.setString("Gold: " + std::to_string(m_gold));
    }
    
    void UIComponent::UpdateChests(int chests)
    {
        m_chests = chests;
        m_chestText.setString("Chests: " + std::to_string(m_chests));
    }
}
