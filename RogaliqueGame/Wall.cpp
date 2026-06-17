#include "Wall.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"

namespace rogalique
{
    Wall::Wall(float x, float y, float width, float height)
    {
        auto* transform = AddComponent<TransformComponent>();
        transform->SetPosition(sf::Vector2f(x, y));
        
        auto* sprite = AddComponent<SpriteComponent>("", width, height);
        sprite->SetFallbackColor(sf::Color(100, 70, 50));
        
        auto* collision = AddComponent<CollisionComponent>(std::max(width, height) / 2.0f);
        collision->SetTrigger(false);
    }

    void Wall::Update(float deltaTime)
    {
        RogaliqueGameObject::Update(deltaTime);
    }
}
