#pragma once
#include "RogaliqueGameObject.h"

namespace rogalique
{
    class Block : public RogaliqueGameObject
    {
    public:
        Block(float x, float y, float width, float height);
        virtual ~Block() = default;
        
        void Update(float deltaTime) override;
        
        virtual sf::Color GetColor() const { return sf::Color(150, 75, 0); } // кирпичный по умолчанию
        
    protected:
        float m_width;
        float m_height;
    };
}
