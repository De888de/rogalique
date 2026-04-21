#pragma once
#include "Block.h"
#include <SFML/Graphics/Text.hpp>

namespace ArkanoidGame
{
    class ToughBlock : public Block  // ← Block уже реализует Colladiable
    {
    public:
        ToughBlock(const sf::Vector2f& position);

        void Draw(sf::RenderWindow& window) override;
        bool CheckCollision(Ball& ball);  
        void OnHit() override;            
        int GetScoreValue() const override { return 10; }

    private:
        int hitsRemaining = 3;
        sf::Font font;
        sf::Text hitCounterText;

        void UpdateHitCounterText();
    };
}

