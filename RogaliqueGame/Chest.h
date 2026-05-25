#pragma once
#include "RogaliqueGameObject.h"

namespace rogalique
{
    class Chest : public RogaliqueGameObject
    {
    public:
        Chest();
        void Update(float deltaTime) override;
        void Render(sf::RenderWindow& window) override;
        
        bool IsCollected() const { return m_isCollected; }
        void Collect();
        
    private:
        sf::RectangleShape m_shape;
        bool m_isCollected = false;
    };
}
