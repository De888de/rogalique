#pragma once
#include <SFML/Graphics.hpp>
#include <string>

namespace rogalique {

class LevelInfoWindow {
public:
    LevelInfoWindow();
    void Show(int levelNumber, int requiredChests);
    void Update(float deltaTime);
    void Render(sf::RenderWindow& window);
    bool IsActive() const { return m_isActive; }
    void Close();

private:
    sf::Text m_titleText;
    sf::Text m_infoText;
    sf::Text m_continueText;
    sf::Font m_font;
    sf::RectangleShape m_background;
    bool m_isActive = false;
    float m_timer = 0.0f;
};

}
