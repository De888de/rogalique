#pragma once
#include "Component.h"
#include <SFML/System/Vector2.hpp>

namespace engine {

class TransformComponent : public Component {
public:
    sf::Vector2f position = {0, 0};
    float rotation = 0.0f;
    sf::Vector2f scale = {1.0f, 1.0f};
    
    void SetPosition(const sf::Vector2f& pos) { position = pos; }
    sf::Vector2f GetPosition() const { return position; }
    
    void SetRotation(float rot) { rotation = rot; }
    float GetRotation() const { return rotation; }
    
    void SetScale(const sf::Vector2f& s) { scale = s; }
    sf::Vector2f GetScale() const { return scale; }
};

} // namespace engine
