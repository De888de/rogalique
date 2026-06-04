#pragma once
#include "Block.h"

namespace rogalique
{
    class MagicBlock : public Block
    {
    public:
        MagicBlock(float x, float y, float width, float height);
        sf::Color GetColor() const override { return sf::Color(200, 50, 200); } // фиолетовый
    };
}
