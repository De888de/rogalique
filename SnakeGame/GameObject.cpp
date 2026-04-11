#include "GameObject.h"
#include "GameSettings.h"
#include <SFML/Graphics/Texture.hpp>

namespace ArkanoidGame
{
    GameObject::GameObject(const std::string& texturePath, const sf::Vector2f& position,
        float width, float height)
    {
        static sf::Texture texture; // упрощённо — в реальном проекте лучше TextureManager
        if (!texture.loadFromFile(texturePath))
        {
            // Можно добавить assert или fallback
        }

        sprite.setTexture(texture);
        sprite.setOrigin(width / 2.0f, height / 2.0f);
        sprite.setPosition(position);
        sprite.setScale(width / texture.getSize().x, height / texture.getSize().y);
    }

    void GameObject::Draw(sf::RenderWindow& window)
    {
        window.draw(sprite);
    }

    sf::FloatRect GameObject::GetRect() const
    {
        return sprite.getGlobalBounds();
    }

    sf::Vector2f GameObject::GetPosition() const
    {
        return sprite.getPosition();
    }

    void GameObject::SetPosition(const sf::Vector2f& pos)
    {
        sprite.setPosition(pos);
    }
}