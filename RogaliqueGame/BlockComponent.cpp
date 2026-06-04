#include "BlockComponent.h"

namespace rogalique
{
    BlockComponent::BlockComponent(RogaliqueGameObject* owner)
        : Component(owner) {}
    
    void BlockComponent::Update(float deltaTime) {}
    void BlockComponent::Render(sf::RenderWindow& window) {}
}
