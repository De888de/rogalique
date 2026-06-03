#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

namespace rogalique
{
    class SoundSettingsWindow
    {
    public:
        SoundSettingsWindow();
        void Update(float deltaTime);
        void Draw(sf::RenderWindow& window);
        void HandleInput(const sf::Event& event);
        
        bool IsActive() const { return m_isActive; }
        void SetActive(bool active) { m_isActive = active; }
        void Reset();
        
        bool IsEffectsEnabled() const { return m_effectsEnabled; }
        int GetSelectedMusicIndex() const { return m_selectedMusicIndex; }
        
    private:
        void MoveUp();
        void MoveDown();
        void UpdateSelection();
        void SwitchAtmosphere(int direction);
        void ToggleEffects();
        
        sf::Font m_font;
        sf::Text m_titleText;
        sf::Text m_musicText;
        sf::Text m_musicOptionText;
        sf::Text m_effectsText;
        sf::Text m_effectsOptionText;
        sf::Text m_backText;
        
        std::vector<sf::Text*> m_options;
        int m_selectedIndex = 0;
        bool m_isActive = true;
        
        bool m_effectsEnabled = true;
        int m_selectedMusicIndex = 0;
        std::string m_currentMusicFile;
        
        std::vector<std::string> m_musicOptions = {
            "Eerie Whispers",
            "Dark Ritual",
            "Horror Awakens"
        };
    };
}
