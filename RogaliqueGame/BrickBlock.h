#pragma once
#include "Block.h"

namespace rogalique
{
    class BrickBlock : public Block
    {
    public:
        BrickBlock(float x, float y, float width, float height);
        sf::Color GetColor() const override { return sf::Color(180, 80, 40); } // красно-кирпичный
    };
}
