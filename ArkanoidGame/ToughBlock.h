#pragma once
#include "Block.h"
#include <SFML/Graphics/Text.hpp>

namespace RoqaliqueGame
{
    class ToughBlock : public Block
    {
    public:
        ToughBlock(const sf::Vector2f& position);

        void Update(float timeDelta) override {}  
        void Draw(sf::RenderWindow& window) override;
        bool CheckCollision(Ball& ball) override;
        void OnHit() override;

        
        int GetScoreValue() const override;

    private:
        int hitsRemaining = 3;
        sf::Font font;
        sf::Text hitCounterText;

        void UpdateHitCounterText();
    };
}