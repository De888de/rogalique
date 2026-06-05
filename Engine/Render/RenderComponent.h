#pragma once
#include "../Core/Component.h"
#include <SFML/Graphics.hpp>

namespace engine {

class RenderComponent : public Component {
public:
    sf::Shape* shape = nullptr;
    sf::Color color = sf::Color::White;
    int layer = 0;
    
    RenderComponent(sf::Shape* s = nullptr) : shape(s) {}
    virtual ~RenderComponent() { delete shape; }
    
    void Render() override {
        if (shape && gameObject) {
            auto* transform = gameObject->GetComponent<TransformComponent>();
            if (transform) {
                shape->setPosition(transform->position);
                shape->setRotation(transform->rotation);
                shape->setScale(transform->scale);
                shape->setFillColor(color);
            }
        }
    }
    
    void SetColor(sf::Color c) { color = c; }
};

class SpriteRenderer : public Component {
public:
    sf::Sprite sprite;
    sf::Texture texture;
    bool textureLoaded = false;
    
    SpriteRenderer(const std::string& texturePath) {
        if (texture.loadFromFile(texturePath)) {
            sprite.setTexture(texture);
            textureLoaded = true;
        }
    }
    
    void Render() override {
        if (textureLoaded && gameObject) {
            auto* transform = gameObject->GetComponent<TransformComponent>();
            if (transform) {
                sprite.setPosition(transform->position);
                sprite.setRotation(transform->rotation);
                sprite.setScale(transform->scale);
            }
        }
    }
    
    sf::Sprite& GetSprite() { return sprite; }
};

} // namespace engine
