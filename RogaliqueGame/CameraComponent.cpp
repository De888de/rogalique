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
        sf::Vector2f viewSize = m_view->getSize();
        
        float leftBound = viewSize.x / 2.0f;
        float rightBound = m_worldWidth - viewSize.x / 2.0f;
        float topBound = viewSize.y / 2.0f;
        float bottomBound = m_worldHeight - viewSize.y / 2.0f;
        
        if (rightBound < leftBound) rightBound = leftBound;
        if (bottomBound < topBound) bottomBound = topBound;
        
        sf::Vector2f desiredCenter = targetPos;
        
        // Ограничиваем
        if (desiredCenter.x < leftBound) desiredCenter.x = leftBound;
        if (desiredCenter.x > rightBound) desiredCenter.x = rightBound;
        if (desiredCenter.y < topBound) desiredCenter.y = topBound;
        if (desiredCenter.y > bottomBound) desiredCenter.y = bottomBound;
        
        m_view->setCenter(desiredCenter);
        
        // Подробная отладка
        static int frame = 0;
        if (frame++ % 30 == 0)
        {
            std::cout << "\n=== CAMERA DEBUG ===" << std::endl;
            std::cout << "Player pos: (" << targetPos.x << ", " << targetPos.y << ")" << std::endl;
            std::cout << "Bounds: L=" << leftBound << " R=" << rightBound 
                      << " T=" << topBound << " B=" << bottomBound << std::endl;
            std::cout << "Center before clamp: (" << targetPos.x << ", " << targetPos.y << ")" << std::endl;
            std::cout << "Center after clamp: (" << desiredCenter.x << ", " << desiredCenter.y << ")" << std::endl;
            
            if (targetPos.x <= leftBound + 10)
                std::cout << ">>> PLAYER AT LEFT BOUNDARY! <<<" << std::endl;
            if (targetPos.x >= rightBound - 10)
                std::cout << ">>> PLAYER AT RIGHT BOUNDARY! <<<" << std::endl;
            if (targetPos.y <= topBound + 10)
                std::cout << ">>> PLAYER AT TOP BOUNDARY! <<<" << std::endl;
            if (targetPos.y >= bottomBound - 10)
                std::cout << ">>> PLAYER AT BOTTOM BOUNDARY! <<<" << std::endl;
        }
    }
    
    void CameraComponent::Render(sf::RenderWindow&) {}
    
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
