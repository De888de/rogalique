#pragma once
#include "Block.h"

namespace RoqaliqueGame
{
    class SmoothDestroyableBlock : public Block
    {
    public:
        SmoothDestroyableBlock(const sf::Vector2f& position);

        void Update(float timeDelta) override;
        void Draw(sf::RenderWindow& window) override;
        void OnHit() override;
        bool CheckCollision(Ball& ball) override;   

    private:
        bool isDying = false;
        float deathTimer = 0.0f;
        float deathDuration = 0.3f;
        sf::Color currentColor;
    };
}