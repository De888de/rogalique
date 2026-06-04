#include "Block.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"

namespace rogalique
{
    Block::Block(float x, float y, float width, float height)
        : m_width(width), m_height(height)
    {
        auto* transform = AddComponent<TransformComponent>();
        transform->SetPosition(sf::Vector2f(x, y));
        
        auto* sprite = AddComponent<SpriteComponent>("", width, height);
        sprite->SetFallbackColor(GetColor());
        
        auto* collision = AddComponent<CollisionComponent>(std::max(width, height) / 2.0f);
        collision->SetTrigger(false);
    }
    
    void Block::Update(float deltaTime)
    {
        RogaliqueGameObject::Update(deltaTime);
    }
}
