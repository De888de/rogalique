#include "Enemy.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "HealthComponent.h"
#include "CollisionComponent.h"
#include "SeekerComponent.h"
#include "GameWorld.h"
#include <iostream>

namespace rogalique
{
    Enemy::Enemy()
    {
        AddComponent<TransformComponent>();
        AddComponent<SpriteComponent>("enemy.png", 32, 32);
        AddComponent<HealthComponent>(m_maxHealth);
        AddComponent<SeekerComponent>(80.0f);
        AddComponent<CollisionComponent>(16.0f);  // добавляем коллизию
        std::cout << "[Enemy] Created with collision" << std::endl;
    }

    void Enemy::Update(float deltaTime)
    {
        RogaliqueGameObject::Update(deltaTime);
        
        HealthComponent* health = GetComponent<HealthComponent>();
        if (health && !health->IsAlive())
        {
            GameWorld::GetInstance().DestroyGameObject(this);
        }
    }

    void Enemy::TakeDamage(int damage)
    {
        HealthComponent* health = GetComponent<HealthComponent>();
        if (health)
            health->TakeDamage(damage);
    }

    void Enemy::Heal(int amount)
    {
        HealthComponent* health = GetComponent<HealthComponent>();
        if (health)
            health->Heal(amount);
    }
}
