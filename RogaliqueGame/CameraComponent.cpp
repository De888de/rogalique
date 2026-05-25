#include "CameraComponent.h"
#include "TransformComponent.h"
#include "RogaliqueGameObject.h"
#include <algorithm>
#include <iostream>

namespace rogalique
{
    CameraComponent::CameraComponent(RogaliqueGameObject* owner)
        : Component(owner)
    {
    }
    
    void CameraComponent::Update(float deltaTime)
    {
        if (!m_target || !m_view) return;
        
        auto* transform = m_target->GetComponent<TransformComponent>();
        if (!transform) return;
        
        sf::Vector2f targetPos = transform->GetPosition();
        
        sf::Vector2f newCenter = targetPos;
        
        if (m_worldWidth > 0 && m_worldHeight > 0)
        {
            sf::Vector2f viewSize = m_view->getSize();
            float left = viewSize.x / 2.0f;
            float right = m_worldWidth - viewSize.x / 2.0f;
            float top = viewSize.y / 2.0f;
            float bottom = m_worldHeight - viewSize.y / 2.0f;
            
            newCenter.x = std::clamp(newCenter.x, left, right);
            newCenter.y = std::clamp(newCenter.y, top, bottom);
        }
        
        m_view->setCenter(newCenter);
        m_offset = newCenter - targetPos;
    }
    
    void CameraComponent::Render(sf::RenderWindow& window) {}
    
    void CameraComponent::SetTarget(RogaliqueGameObject* target)
    {
        m_target = target;
        std::cout << "[Camera] Target set" << std::endl;
    }
    
    void CameraComponent::SetBounds(float worldWidth, float worldHeight)
    {
        m_worldWidth = worldWidth;
        m_worldHeight = worldHeight;
        std::cout << "[Camera] Bounds set: " << worldWidth << "x" << worldHeight << std::endl;
    }
    
    void CameraComponent::SetViewport(sf::View& view)
    {
        m_view = &view;
    }
}
