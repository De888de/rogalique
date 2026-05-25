#include "RogaliqueGameObject.h"
#include "Component.h"
#include <iostream>

namespace rogalique
{
    RogaliqueGameObject::RogaliqueGameObject() : m_position(0, 0) 
    {
        std::cout << "[GameObject] Created" << std::endl;
    }
    
    RogaliqueGameObject::~RogaliqueGameObject() = default;

    void RogaliqueGameObject::Update(float deltaTime)
    {
        static int frame = 0;
        frame++;
        if (frame % 60 == 0)
            std::cout << "[GameObject] Update, components=" << m_components.size() << std::endl;
        
        for (auto& comp : m_components)
            comp->Update(deltaTime);
    }

    void RogaliqueGameObject::Render(sf::RenderWindow& window)
    {
        for (auto& comp : m_components)
            comp->Render(window);
    }
}
