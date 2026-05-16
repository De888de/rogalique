#pragma once
#include <SFML/Graphics.hpp>
#include <string>

namespace ArkanoidGame
{
    class GameObject
    {
    protected:
        sf::Sprite sprite;
        sf::Texture texture;
        bool hasTexture = false;   

    public:
        GameObject(const std::string& texturePath, const sf::Vector2f& position,
            float width, float height);

        virtual ~GameObject() = default;

        virtual void Update(float timeDelta) = 0;
        virtual void Draw(sf::RenderWindow& window);

        sf::FloatRect GetRect() const;
        sf::Vector2f GetPosition() const;
        void SetPosition(const sf::Vector2f& pos);

        sf::Sprite& GetSprite();   // ← только объявление, без тела { return sprite; }

    };
}