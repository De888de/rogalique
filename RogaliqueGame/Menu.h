#pragma once
#include <SFML/Graphics.hpp>

namespace rogalique
{
    class Menu
    {
    public:
        Menu();
        void Update(float deltaTime);
        void Draw(sf::RenderWindow& window);
        void HandleInput(const sf::Event& event);
        
        bool IsPlaySelected() const { return m_playSelected; }
        bool IsExitSelected() const { return m_exitSelected; }
        void Reset();
        
    private:
        void MoveUp();
        void MoveDown();
        void UpdateSelection();
        
        sf::Font m_font;
        sf::Text m_titleText;
        sf::Text m_playText;
        sf::Text m_exitText;
        sf::Text m_quoteText;  // Королевский девиз
        
        int m_selectedIndex = 0;
        bool m_playSelected = false;
        bool m_exitSelected = false;
    };
}
