#include "Player.h"
#include "Chest.h"
#include "GameWorld.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "MovementComponent.h"
#include "HealthComponent.h"
#include <cmath>
#include <iostream>

namespace rogalique
{
    Player::Player()
    {
        AddComponent<TransformComponent>();
        AddComponent<SpriteComponent>("player.png", 32, 32);
        AddComponent<MovementComponent>(200.0f);
        AddComponent<HealthComponent>(100);
    }
    
    void Player::Update(float deltaTime)
    {
        RogaliqueGameObject::Update(deltaTime);
        
        // Проверка сбора сундуков
        auto& world = GameWorld::GetInstance();
        for (auto* obj : world.GetAllGameObjects())
        {
            auto* chest = dynamic_cast<Chest*>(obj);
            if (chest && !chest->IsCollected())
            {
                float dx = m_position.x - chest->GetPosition().x;
                float dy = m_position.y - chest->GetPosition().y;
                float dist = std::sqrt(dx*dx + dy*dy);
                if (dist < 32.0f)
                {
                    chest->Collect();
                }
            }
        }
    }
}
