#include "LevelInfoWindow.h"
#include <iostream>

namespace rogalique {

LevelInfoWindow::LevelInfoWindow() {
    if (!m_font.loadFromFile("RogaliqueGame/Resources/Fonts/Roboto-Regular.ttf")) {
        std::cout << "[LevelInfoWindow] Could not load font!" << std::endl;
    }
    
    m_background.setSize(sf::Vector2f(600, 300));
    m_background.setFillColor(sf::Color(0, 0, 0, 200));
    m_background.setOutlineColor(sf::Color(255, 215, 0, 255));
    m_background.setOutlineThickness(3);
    m_background.setOrigin(300, 150);
}

void LevelInfoWindow::Show(int levelNumber, int requiredChests) {
    m_isActive = true;
    m_timer = 0.0f;
    
    m_titleText.setFont(m_font);
    m_titleText.setString("LEVEL " + std::to_string(levelNumber));
    m_titleText.setCharacterSize(48);
    m_titleText.setFillColor(sf::Color::White);
    m_titleText.setOrigin(m_titleText.getLocalBounds().width / 2, m_titleText.getLocalBounds().height / 2);
    
    m_infoText.setFont(m_font);
    m_infoText.setString("Find " + std::to_string(requiredChests) + " chests to open the portal!");
    m_infoText.setCharacterSize(28);
    m_infoText.setFillColor(sf::Color(255, 215, 0));
    m_infoText.setOrigin(m_infoText.getLocalBounds().width / 2, m_infoText.getLocalBounds().height / 2);
    
    m_continueText.setFont(m_font);
    m_continueText.setString("Press [SPACE] to continue...");
    m_continueText.setCharacterSize(20);
    m_continueText.setFillColor(sf::Color(150, 150, 150));
    m_continueText.setOrigin(m_continueText.getLocalBounds().width / 2, m_continueText.getLocalBounds().height / 2);
}

void LevelInfoWindow::Update(float deltaTime) {
    if (m_isActive) {
        m_timer += deltaTime;
    }
}

void LevelInfoWindow::Render(sf::RenderWindow& window) {
    if (!m_isActive) return;
    
    sf::Vector2f center = window.getDefaultView().getCenter();
    m_background.setPosition(center);
    window.draw(m_background);
    
    m_titleText.setPosition(center.x, center.y - 80);
    window.draw(m_titleText);
    
    m_infoText.setPosition(center.x, center.y - 20);
    window.draw(m_infoText);
    
    if (fmod(m_timer, 1.0f) < 0.5f) {
        m_continueText.setPosition(center.x, center.y + 60);
        window.draw(m_continueText);
    }
}

void LevelInfoWindow::Close() {
    m_isActive = false;
}

}
