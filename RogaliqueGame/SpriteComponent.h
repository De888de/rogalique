#pragma once
#include "Component.h"
#include <string>

namespace rogalique
{
    class SpriteComponent : public Component
    {
    public:
        SpriteComponent(RogaliqueGameObject* owner, const std::string& texturePath, float width, float height);
        void Update(float deltaTime) override;
        void Render(sf::RenderWindow& window) override;
        
        void SetPosition(const sf::Vector2f& pos);
        
    private:
        sf::Texture m_texture;
        sf::Sprite m_sprite;
        sf::RectangleShape m_fallbackShape;
        float m_width;
        float m_height;
        bool m_hasTexture;
    };
}
