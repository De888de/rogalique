#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "WeaponItem.h"
#include <cmath>
#include "GameWorld.h"
#include "Chest.h"
#include "CollisionComponent.h"
#include "TransformComponent.h"
#include "AttackComponent.h"
#include "Application.h"
#include <algorithm>
#include <iostream>
#include <random>

namespace rogalique
{
    GameWorld& GameWorld::GetInstance()
    {
        static GameWorld instance;
        return instance;
    }
    
    void GameWorld::Update(float deltaTime)
    {
        for (auto* obj : m_gameObjects)
        {
            if (obj)
                obj->Update(deltaTime);
        }

        // Логика подбора оружия
        if (m_player) {
            for (auto it = m_gameObjects.begin(); it != m_gameObjects.end(); ) {
                WeaponItem* weapon = dynamic_cast<WeaponItem*>(*it);
                if (weapon && weapon->CheckPickup(m_player->GetPosition())) {
                    m_player->EquipWeapon();
                    delete *it;
                    it = m_gameObjects.erase(it);
                    continue;
                }
                ++it;
            }
        }
    }
    
    void GameWorld::Render(sf::RenderWindow& window)
    {
        for (auto* obj : m_gameObjects)
        {
            if (obj)
                obj->Render(window);
        }
    }
    
    void GameWorld::LateUpdate()
    {
        for (auto* obj : m_markedForDestroy)
        {
            auto it = std::find(m_gameObjects.begin(), m_gameObjects.end(), obj);
            if (it != m_gameObjects.end())
            {
                m_gameObjects.erase(it);
                delete obj;
            }
        }
        m_markedForDestroy.clear();
    }
    
    void GameWorld::DestroyGameObject(RogaliqueGameObject* obj)
    {
        if (std::find(m_markedForDestroy.begin(), m_markedForDestroy.end(), obj) == m_markedForDestroy.end())
        {
            m_markedForDestroy.push_back(obj);
        }
    }
    
    void GameWorld::DestroyGameObjectImmediate(RogaliqueGameObject* obj)
    {
        auto it = std::find(m_gameObjects.begin(), m_gameObjects.end(), obj);
        if (it != m_gameObjects.end())
        {
            m_gameObjects.erase(it);
            delete obj;
        }
    }
    
    void GameWorld::Clear()
    {
        for (auto* obj : m_gameObjects)
            delete obj;
        m_gameObjects.clear();
        m_markedForDestroy.clear();
        m_player = nullptr;
    }
    
    void GameWorld::SpawnChests(int count, float worldWidth, float worldHeight)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> distX(50, worldWidth - 50);
        std::uniform_real_distribution<float> distY(50, worldHeight - 50);
        
        int spawned = 0;
        int attempts = 0;
        const int maxAttempts = 500;
        
        while (spawned < count && attempts < maxAttempts)
        {
            float x = distX(gen);
            float y = distY(gen);
            sf::Vector2f pos(x, y);
            
            if (IsPositionFree(pos, 16.0f))
            {
                auto* chest = CreateGameObject<Chest>();
                chest->SetPosition(pos);
                spawned++;
                std::cout << "[GameWorld] Spawned chest " << spawned << " at (" << x << ", " << y << ")" << std::endl;
            }
            attempts++;
        }
        
        if (spawned < count)
        {
            std::cout << "[GameWorld] Warning: Only spawned " << spawned << " chests out of " << count << std::endl;
        }
    }
    
    bool GameWorld::IsPositionFree(const sf::Vector2f& pos, float radius) const
    {
        for (auto* obj : m_gameObjects)
        {
            auto* collision = obj->GetComponent<CollisionComponent>();
            if (collision)
            {
                auto* transform = obj->GetComponent<TransformComponent>();
                if (transform)
                {
                    sf::Vector2f objPos = transform->GetPosition();
                    float dx = pos.x - objPos.x;
                    float dy = pos.y - objPos.y;
                    float dist = std::sqrt(dx * dx + dy * dy);
                    if (dist < radius + collision->GetRadius())
                    {
                        return false;
                    }
                }
            }
        }
        return true;
    }
    
    void GameWorld::CheckCollisions()
    {
        std::vector<CollisionComponent*> colliders;

        // 1. Коллизии через CollisionComponent
        for (size_t i = 0; i < m_gameObjects.size(); ++i)
        {
            CollisionComponent* col = m_gameObjects[i]->GetComponent<CollisionComponent>();
            if (col)
                colliders.push_back(col);
        }

        for (size_t i = 0; i < colliders.size(); ++i)
        {
            for (size_t j = i + 1; j < colliders.size(); ++j)
            {
                if (colliders[i]->CheckCollision(colliders[j]))
                {
                    colliders[i]->OnCollisionEnter(colliders[j]);
                    colliders[j]->OnCollisionEnter(colliders[i]);
                }
            }
        }

        // 2. Пули vs Враги
        for (size_t i = 0; i < m_gameObjects.size(); ++i)
        {
            Bullet* bullet = dynamic_cast<Bullet*>(m_gameObjects[i]);
            if (!bullet) continue;

            for (size_t j = 0; j < m_gameObjects.size(); ++j)
            {
                Enemy* enemy = dynamic_cast<Enemy*>(m_gameObjects[j]);
                if (!enemy) continue;

                auto* bulletTransform = bullet->GetComponent<TransformComponent>();
                auto* enemyTransform = enemy->GetComponent<TransformComponent>();

                if (!bulletTransform || !enemyTransform) continue;

                sf::Vector2f bulletPos = bulletTransform->GetPosition();
                sf::Vector2f enemyPos = enemyTransform->GetPosition();

                float dx = bulletPos.x - enemyPos.x;
                float dy = bulletPos.y - enemyPos.y;
                float dist = std::sqrt(dx * dx + dy * dy);

                if (dist < 25.0f)
                {
                    int damage = enemy->GetMaxHealth() * 0.25f;
                    if (damage < 1) damage = 1;

                    enemy->TakeDamage(damage);
                    std::cout << "[Collision] BULLET HIT! Damage: " << damage << std::endl;
                    DestroyGameObject(bullet);
                    break;
                }
            }
        }
        
        // 3. Игрок vs Враги - атака через AttackComponent
        if (m_player && m_player->IsAlive())
        {
            std::cout << "[Collision] === ENEMY ATTACK CHECK ===" << std::endl;
            std::cout << "[Collision] Player HP: " << m_player->GetHealth() << std::endl;

            int enemyCount = 0;
            for (auto* enemyObj : m_gameObjects)
            {
                Enemy* enemy = dynamic_cast<Enemy*>(enemyObj);
                if (!enemy) continue;
                if (!enemy->IsAlive()) {
                    std::cout << "[Collision] Enemy " << enemyCount << " is dead" << std::endl;
                    continue;
                }

                enemyCount++;
                std::cout << "[Collision] Enemy " << enemyCount << " is alive" << std::endl;

                auto* attackComp = enemy->GetComponent<AttackComponent>();
                if (!attackComp) {
                    std::cout << "[Collision] Enemy " << enemyCount << " has NO AttackComponent!" << std::endl;
                    continue;
                }

                bool canAttack = attackComp->CanAttack();
                bool inRange = attackComp->IsInRange(m_player);

                std::cout << "[Collision] Enemy " << enemyCount
                    << ": canAttack=" << canAttack
                    << ", inRange=" << inRange << std::endl;

                if (canAttack && inRange)
                {
                    std::cout << "[Collision] Enemy " << enemyCount << " ATTACKING!" << std::endl;
                    attackComp->Attack(m_player);
                    std::cout << "[Collision] After attack, Player HP: " << m_player->GetHealth() << std::endl;
                    break;
                }
            }
            std::cout << "[Collision] Total enemies checked: " << enemyCount << std::endl;
        }
    }
}
