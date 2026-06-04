#include "BlockBuilder.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include "BlockComponent.h"

namespace rogalique
{
    RogaliqueGameObject* BlockBuilder::CreateBlock(float x, float y, float width, float height, bool isMagic)
    {
        auto* block = GameWorld::GetInstance().CreateGameObject<RogaliqueGameObject>();
        
        // Transform
        auto* transform = block->AddComponent<TransformComponent>();
        transform->SetPosition(sf::Vector2f(x, y));
        
        // Visual
        auto* sprite = block->AddComponent<SpriteComponent>("", width, height);
        if (isMagic)
            sprite->SetFallbackColor(sf::Color(200, 50, 200)); // фиолетовый
        else
            sprite->SetFallbackColor(sf::Color(180, 80, 40));  // кирпичный
        
        // Collision
        auto* collision = block->AddComponent<CollisionComponent>(std::max(width, height) / 2.0f);
        collision->SetTrigger(false);
        
        // Marker
        auto* marker = block->AddComponent<BlockComponent>();
        marker->SetMagic(isMagic);
        
        return block;
    }
    
    void BlockBuilder::BuildPerimeter(float worldWidth, float worldHeight, float thickness)
    {
        float halfThick = thickness / 2.0f;
        
        // Верхняя стена
        for (float x = halfThick; x < worldWidth - halfThick; x += thickness)
            CreateBlock(x, halfThick, thickness, thickness, false);
        
        // Нижняя стена
        for (float x = halfThick; x < worldWidth - halfThick; x += thickness)
            CreateBlock(x, worldHeight - halfThick, thickness, thickness, false);
        
        // Левая стена
        for (float y = halfThick; y < worldHeight - halfThick; y += thickness)
            CreateBlock(halfThick, y, thickness, thickness, false);
        
        // Правая стена
        for (float y = halfThick; y < worldHeight - halfThick; y += thickness)
            CreateBlock(worldWidth - halfThick, y, thickness, thickness, false);
    }
    
    void BlockBuilder::BuildRow(float y, float startX, float endX, float blockSize, bool isMagic)
    {
        for (float x = startX; x < endX; x += blockSize)
            CreateBlock(x, y, blockSize, blockSize, isMagic);
    }
}
