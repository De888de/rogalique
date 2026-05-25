#include "RogaliqueGameObject.h"
#include "Component.h"

namespace rogalique
{
    RogaliqueGameObject::RogaliqueGameObject() : m_position(0, 0) {}
    RogaliqueGameObject::~RogaliqueGameObject() = default;
    
    void RogaliqueGameObject::Update(float deltaTime)
    {
        for (auto& comp : m_components)
            comp->Update(deltaTime);
    }
    
    void RogaliqueGameObject::Render(sf::RenderWindow& window)
    {
        for (auto& comp : m_components)
            comp->Render(window);
    }
}
