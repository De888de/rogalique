#include "WallComponent.h"
#include <iostream>

namespace rogalique
{
    WallComponent::WallComponent(RogaliqueGameObject* owner)
        : Component(owner)
    {
        std::cout << "[WallComponent] Created" << std::endl;
    }

    void WallComponent::Update(float deltaTime)
    {
        (void)deltaTime;
    }

    void WallComponent::Render(sf::RenderWindow& window)
    {
        (void)window;
    }
}
