#pragma once
#include "Component.h"
#include <SFML/Graphics.hpp>

namespace rogalique
{
    class CameraComponent : public Component
    {
    public:
        CameraComponent(RogaliqueGameObject* owner);
        void Update(float deltaTime) override;
        void Render(sf::RenderWindow& window) override;
        
        void SetTarget(RogaliqueGameObject* target);
        void SetBounds(float worldWidth, float worldHeight);
        void SetViewport(sf::View& view);
        
        sf::Vector2f GetOffset() const { return m_offset; }
        
    private:
        RogaliqueGameObject* m_target = nullptr;
        sf::View* m_view = nullptr;
        sf::Vector2f m_offset;
        float m_worldWidth = 0;
        float m_worldHeight = 0;
    };
}
