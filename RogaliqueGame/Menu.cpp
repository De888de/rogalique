#include "Menu.h"
#include <iostream>

namespace rogalique
{
    Menu::Menu()
    {
        if (!m_font.loadFromFile("D:/xyz/roqalique/RogaliqueGame/Resources/Fonts/Roboto-Regular.ttf"))
        {
            std::cout << "[Menu] Warning: Could not load font" << std::endl;
        }
        
        // Заголовок
        m_titleText.setFont(m_font);
        m_titleText.setString("ROGALIQUE");
        m_titleText.setCharacterSize(64);
        m_titleText.setFillColor(sf::Color::White);
        m_titleText.setOrigin(m_titleText.getLocalBounds().width / 2.0f, m_titleText.getLocalBounds().height / 2.0f);
        m_titleText.setPosition(512, 150);
        
        // Пункты меню
        m_playText.setFont(m_font);
        m_playText.setString("START HUNT");
        m_playText.setCharacterSize(36);
        m_playText.setOrigin(m_playText.getLocalBounds().width / 2.0f, m_playText.getLocalBounds().height / 2.0f);
        m_playText.setPosition(512, 350);
        
        m_exitText.setFont(m_font);
        m_exitText.setString("LEAVE THE PATH");
        m_exitText.setCharacterSize(36);
        m_exitText.setOrigin(m_exitText.getLocalBounds().width / 2.0f, m_exitText.getLocalBounds().height / 2.0f);
        m_exitText.setPosition(512, 420);
        
        // Королевский девиз (мелко)
        m_quoteText.setFont(m_font);
        m_quoteText.setString("Long live the Rogalique King!");
        m_quoteText.setCharacterSize(16);
        m_quoteText.setFillColor(sf::Color(180, 180, 180));
        m_quoteText.setOrigin(m_quoteText.getLocalBounds().width / 2.0f, m_quoteText.getLocalBounds().height / 2.0f);
        m_quoteText.setPosition(512, 550);
        
        UpdateSelection();
    }
    
    void Menu::Update(float) {}
    
    void Menu::Draw(sf::RenderWindow& window)
    {
        window.draw(m_titleText);
        window.draw(m_playText);
        window.draw(m_exitText);
        window.draw(m_quoteText);
    }
    
    void Menu::HandleInput(const sf::Event& event)
    {
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Up) MoveUp();
            else if (event.key.code == sf::Keyboard::Down) MoveDown();
            else if (event.key.code == sf::Keyboard::Enter)
            {
                if (m_selectedIndex == 0) m_playSelected = true;
                else if (m_selectedIndex == 1) m_exitSelected = true;
            }
            else if (event.key.code == sf::Keyboard::Escape)
            {
                m_exitSelected = true;
            }
        }
    }
    
    void Menu::MoveUp()
    {
        if (m_selectedIndex > 0)
        {
            m_selectedIndex--;
            UpdateSelection();
        }
    }
    
    void Menu::MoveDown()
    {
        if (m_selectedIndex < 1)
        {
            m_selectedIndex++;
            UpdateSelection();
        }
    }
    
    void Menu::UpdateSelection()
    {
        m_playText.setFillColor(m_selectedIndex == 0 ? sf::Color::Yellow : sf::Color::White);
        m_exitText.setFillColor(m_selectedIndex == 1 ? sf::Color::Yellow : sf::Color::White);
    }
    
    void Menu::Reset()
    {
        m_playSelected = false;
        m_exitSelected = false;
        m_selectedIndex = 0;
        UpdateSelection();
    }
}
