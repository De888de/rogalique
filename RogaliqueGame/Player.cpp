#include "Player.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "MovementComponent.h"
#include "HealthComponent.h"

namespace rogalique
{
    Player::Player()
    {
        AddComponent<TransformComponent>();
        AddComponent<SpriteComponent>("player.png", 32, 32);
        AddComponent<MovementComponent>(200.0f);
        AddComponent<HealthComponent>(100);
    }
}
