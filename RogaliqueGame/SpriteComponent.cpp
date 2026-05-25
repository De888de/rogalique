#include "SpriteComponent.h"
#include "TransformComponent.h"
#include "RogaliqueGameObject.h"
#include <iostream>

namespace rogalique
{
    SpriteComponent::SpriteComponent(RogaliqueGameObject* owner, const std::string& texturePath, float width, float height)
        : Component(owner), m_width(width), m_height(height), m_hasTexture(false)
    {
        if (!m_texture.loadFromFile(texturePath))
        {
            std::cout << "[SpriteComponent] Failed to load texture: " << texturePath << std::endl;
            m_hasTexture = false;
            m_fallbackShape.setSize(sf::Vector2f(width, height));
            m_fallbackShape.setFillColor(sf::Color::Green);
            m_fallbackShape.setOrigin(width / 2.0f, height / 2.0f);
        }
        else
        {
            m_hasTexture = true;
            m_sprite.setTexture(m_texture);
            float scaleX = width / m_texture.getSize().x;
            float scaleY = height / m_texture.getSize().y;
            m_sprite.setScale(scaleX, scaleY);
            m_sprite.setOrigin(width / 2.0f, height / 2.0f);
        }
    }
    
    void SpriteComponent::Update(float deltaTime)
    {
        // Берём позицию из TransformComponent
        auto* transform = m_owner->GetComponent<TransformComponent>();
        if (transform)
        {
            sf::Vector2f pos = transform->GetPosition();
            if (m_hasTexture)
                m_sprite.setPosition(pos);
            else
                m_fallbackShape.setPosition(pos);
        }
    }
    
    void SpriteComponent::Render(sf::RenderWindow& window)
    {
        if (m_hasTexture)
            window.draw(m_sprite);
        else
            window.draw(m_fallbackShape);
    }
    
    void SpriteComponent::SetPosition(const sf::Vector2f& pos)
    {
        if (m_hasTexture)
            m_sprite.setPosition(pos);
        else
            m_fallbackShape.setPosition(pos);
    }
}
