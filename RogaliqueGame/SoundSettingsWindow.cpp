#include "SoundSettingsWindow.h"
#include "SoundManager.h"
#include <iostream>

namespace rogalique
{
    SoundSettingsWindow::SoundSettingsWindow()
    {
        if (!m_font.loadFromFile("D:/xyz/roqalique/RogaliqueGame/Resources/Fonts/Roboto-Regular.ttf"))
        {
            std::cout << "[SoundSettings] Warning: Could not load font" << std::endl;
        }
        
        m_titleText.setFont(m_font);
        m_titleText.setString("SOUND SETTINGS");
        m_titleText.setCharacterSize(48);
        m_titleText.setFillColor(sf::Color::White);
        m_titleText.setOrigin(m_titleText.getLocalBounds().width / 2.0f, m_titleText.getLocalBounds().height / 2.0f);
        m_titleText.setPosition(512, 100);
        
        auto setupText = [this](sf::Text& text, const std::string& str, float y, int size = 32)
        {
            text.setFont(m_font);
            text.setString(str);
            text.setCharacterSize(size);
            text.setOrigin(text.getLocalBounds().width / 2.0f, text.getLocalBounds().height / 2.0f);
            text.setPosition(512, y);
        };
        
        setupText(m_musicText, "ATMOSPHERE:", 250, 28);
        setupText(m_musicOptionText, m_musicOptions[0] + " (available)", 310, 24);
        setupText(m_effectsText, "EFFECTS:", 390, 28);
        setupText(m_effectsOptionText, "ON", 450, 24);
        setupText(m_backText, "BACK", 550, 32);
        
        m_options = { &m_musicOptionText, &m_effectsOptionText, &m_backText };
        m_selectedIndex = 0;
        UpdateSelection();
        
        m_selectedMusicIndex = 0;
        m_effectsEnabled = true;
    }
    
    void SoundSettingsWindow::Update(float) {}
    
    void SoundSettingsWindow::Draw(sf::RenderWindow& window)
    {
        window.draw(m_titleText);
        window.draw(m_musicText);
        window.draw(m_musicOptionText);
        window.draw(m_effectsText);
        window.draw(m_effectsOptionText);
        window.draw(m_backText);
    }
    
    void SoundSettingsWindow::HandleInput(const sf::Event& event)
    {
        if (!m_isActive) return;
        
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Up) MoveUp();
            else if (event.key.code == sf::Keyboard::Down) MoveDown();
            else if (event.key.code == sf::Keyboard::Left)
            {
                if (m_selectedIndex == 0)
                    SwitchAtmosphere(-1);
                else if (m_selectedIndex == 1)
                    ToggleEffects();
            }
            else if (event.key.code == sf::Keyboard::Right)
            {
                if (m_selectedIndex == 0)
                    SwitchAtmosphere(1);
                else if (m_selectedIndex == 1)
                    ToggleEffects();
            }
            else if (event.key.code == sf::Keyboard::Enter && m_selectedIndex == 2)
            {
                SoundManager::GetInstance().PlaySound("click");
                m_isActive = false;
            }
            else if (event.key.code == sf::Keyboard::Escape)
            {
                m_isActive = false;
            }
        }
    }
    
    void SoundSettingsWindow::SwitchAtmosphere(int direction)
    {
        int newIndex = m_selectedMusicIndex + direction;
        
        if (newIndex < 0) newIndex = 1;
        if (newIndex > 1) newIndex = 0;
        
        if (newIndex == m_selectedMusicIndex) return;
        
        m_selectedMusicIndex = newIndex;
        m_musicOptionText.setString(m_musicOptions[m_selectedMusicIndex] + " (available)");
        
        // Меняем музыку через PlayMusicFile с правильным путём
        std::string musicFile = (m_selectedMusicIndex == 0) 
            ? "D:/xyz/roqalique/RogaliqueGame/Resources/Sounds/atmosphere_eerie.WAV"
            : "D:/xyz/roqalique/RogaliqueGame/Resources/Sounds/atmosphere_dark.WAV";
        
        std::cout << "[SoundSettings] Playing: " << musicFile << std::endl;
        SoundManager::GetInstance().PlayMusicFile(musicFile);
        SoundManager::GetInstance().PlaySound("click");
        
        std::cout << "[SoundSettings] Switched to: " << m_musicOptions[m_selectedMusicIndex] << std::endl;
    }
    
    void SoundSettingsWindow::ToggleEffects()
    {
        m_effectsEnabled = !m_effectsEnabled;
        m_effectsOptionText.setString(m_effectsEnabled ? "ON" : "OFF");
        SoundManager::GetInstance().SetSoundVolume(m_effectsEnabled ? 100.0f : 0.0f);
        SoundManager::GetInstance().PlaySound("click");
    }
    
    void SoundSettingsWindow::MoveUp()
    {
        if (m_selectedIndex > 0)
        {
            m_selectedIndex--;
            UpdateSelection();
        }
    }
    
    void SoundSettingsWindow::MoveDown()
    {
        if (m_selectedIndex < static_cast<int>(m_options.size()) - 1)
        {
            m_selectedIndex++;
            UpdateSelection();
        }
    }
    
    void SoundSettingsWindow::UpdateSelection()
    {
        for (size_t i = 0; i < m_options.size(); ++i)
        {
            m_options[i]->setFillColor(i == m_selectedIndex ? sf::Color::Yellow : sf::Color::White);
        }
    }
    
    void SoundSettingsWindow::Reset()
    {
        m_selectedIndex = 0;
        UpdateSelection();
        m_isActive = true;
    }
}
