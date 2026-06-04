#pragma once
#include "Component.h"

namespace rogalique
{
    // Компонент-маркер для стен
    class WallComponent : public Component
    {
    public:
        WallComponent(RogaliqueGameObject* owner);
        void Update(float deltaTime) override;
        void Render(sf::RenderWindow& window) override;
    };
}
