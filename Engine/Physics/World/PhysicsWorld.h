#pragma once
#include "../../Core/Component.h"
#include "Vector2.h"        // если внутри Engine
#include <vector>
#include <functional>

namespace engine {

class RigidbodyComponent;
class Collider;

struct CollisionManifold {
    RigidbodyComponent* bodyA;
    RigidbodyComponent* bodyB;
    sf::Vector2f normal;
    float penetration;
    
    CollisionManifold(RigidbodyComponent* a, RigidbodyComponent* b)
        : bodyA(a), bodyB(b), penetration(0) {}
};

class PhysicsWorld {
public:
    PhysicsWorld();
    
    void Update(float deltaTime);
    void AddRigidbody(RigidbodyComponent* body);
    void RemoveRigidbody(RigidbodyComponent* body);
    
    void SetGravity(sf::Vector2f gravity) { gravity_ = gravity; }
    sf::Vector2f GetGravity() const { return gravity_; }
    
    void SetIterations(int iterations) { resolutionIterations_ = iterations; }
    
    using CollisionCallback = std::function<void(CollisionManifold&)>;
    void SetCollisionCallback(CollisionCallback callback) { collisionCallback_ = callback; }
    
private:
    std::vector<RigidbodyComponent*> rigidbodies_;
    sf::Vector2f gravity_ = {0, -9.81f};
    int resolutionIterations_ = 5;
    CollisionCallback collisionCallback_;
    
    void ApplyGravity();
    void DetectCollisions(std::vector<CollisionManifold>& manifolds);
    void ResolveCollisions(std::vector<CollisionManifold>& manifolds);
    void ResolveCollision(CollisionManifold& manifold);
    void PositionalCorrection(CollisionManifold& manifold);
};

} 
