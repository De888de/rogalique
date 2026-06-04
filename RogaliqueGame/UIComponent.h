#pragma once
#include "Component.h"
#include <SFML/Graphics.hpp>

namespace rogalique
{
    class UIComponent : public Component
    {
    public:
        UIComponent(RogaliqueGameObject* owner);
        void Update(float deltaTime) override;
        void Render(sf::RenderWindow& window) override;
        
        void UpdateGold(int gold);
        void UpdateChests(int chests);
        
    private:
        sf::Font m_font;
        sf::Text m_goldText;
        sf::Text m_chestText;
        int m_gold = 0;
        int m_chests = 0;
    };
}
