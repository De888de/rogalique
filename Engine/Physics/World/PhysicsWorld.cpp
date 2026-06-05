#include "PhysicsWorld.h"
#include "../RigidbodyComponent.h"
#include "../Colliders/Collider.h"
#include "../../Core/TransformComponent.h"
#include <algorithm>
#include <cmath>

namespace engine {

PhysicsWorld::PhysicsWorld() = default;

void PhysicsWorld::AddRigidbody(RigidbodyComponent* body) {
    if (body && std::find(rigidbodies_.begin(), rigidbodies_.end(), body) == rigidbodies_.end()) {
        rigidbodies_.push_back(body);
    }
}

void PhysicsWorld::RemoveRigidbody(RigidbodyComponent* body) {
    auto it = std::find(rigidbodies_.begin(), rigidbodies_.end(), body);
    if (it != rigidbodies_.end()) {
        rigidbodies_.erase(it);
    }
}

void PhysicsWorld::ApplyGravity() {
    for (auto* body : rigidbodies_) {
        if (body && !body->IsKinematic()) {
            body->ApplyForce(gravity_ * body->GetMass() * body->GetGravityScale());
        }
    }
}

void PhysicsWorld::DetectCollisions(std::vector<CollisionManifold>& manifolds) {
    manifolds.clear();
    
    for (size_t i = 0; i < rigidbodies_.size(); i++) {
        for (size_t j = i + 1; j < rigidbodies_.size(); j++) {
            auto* bodyA = rigidbodies_[i];
            auto* bodyB = rigidbodies_[j];
            
            if (!bodyA || !bodyB) continue;
            if (bodyA->IsKinematic() && bodyB->IsKinematic()) continue;
            
            auto* colliderA = bodyA->GetCollider();
            auto* colliderB = bodyB->GetCollider();
            
            if (colliderA && colliderB && !colliderA->isTrigger && !colliderB->isTrigger) {
                CollisionManifold manifold(bodyA, bodyB);
                if (colliderA->CheckCollision(colliderB, manifold.normal, manifold.penetration)) {
                    manifolds.push_back(manifold);
                    if (collisionCallback_) {
                        collisionCallback_(manifold);
                    }
                }
            }
        }
    }
}

void PhysicsWorld::ResolveCollision(CollisionManifold& manifold) {
    auto* bodyA = manifold.bodyA;
    auto* bodyB = manifold.bodyB;
    
    if (bodyA->IsKinematic() && bodyB->IsKinematic()) return;
    
    sf::Vector2f relativeVelocity = bodyB->GetLinearVelocity() - bodyA->GetLinearVelocity();
    float velocityAlongNormal = relativeVelocity.x * manifold.normal.x + relativeVelocity.y * manifold.normal.y;
    
    if (velocityAlongNormal > 0) return;
    
    float e = 0.5f;
    float invMassSum = bodyA->GetInvMass() + bodyB->GetInvMass();
    
    if (invMassSum == 0) return;
    
    float impulseMagnitude = -(1 + e) * velocityAlongNormal / invMassSum;
    sf::Vector2f impulse = manifold.normal * impulseMagnitude;
    
    bodyA->ApplyImpulse(-impulse);
    bodyB->ApplyImpulse(impulse);
}

void PhysicsWorld::PositionalCorrection(CollisionManifold& manifold) {
    const float percent = 0.2f;
    const float slop = 0.01f;
    
    float correctionMagnitude = std::max(manifold.penetration - slop, 0.0f) * percent;
    sf::Vector2f correction = manifold.normal * correctionMagnitude;
    
    float invMassSum = manifold.bodyA->GetInvMass() + manifold.bodyB->GetInvMass();
    if (invMassSum == 0) return;
    
    if (!manifold.bodyA->IsKinematic()) {
        auto* transformA = manifold.bodyA->GetTransform();
        if (transformA) transformA->position -= correction * (manifold.bodyA->GetInvMass() / invMassSum);
    }
    if (!manifold.bodyB->IsKinematic()) {
        auto* transformB = manifold.bodyB->GetTransform();
        if (transformB) transformB->position += correction * (manifold.bodyB->GetInvMass() / invMassSum);
    }
}

void PhysicsWorld::ResolveCollisions(std::vector<CollisionManifold>& manifolds) {
    for (int iter = 0; iter < resolutionIterations_; iter++) {
        for (auto& manifold : manifolds) {
            ResolveCollision(manifold);
        }
    }
    
    for (auto& manifold : manifolds) {
        PositionalCorrection(manifold);
    }
}

void PhysicsWorld::Update(float deltaTime) {
    if (deltaTime > 0.033f) deltaTime = 0.033f;
    
    ApplyGravity();
    
    for (auto* body : rigidbodies_) {
        if (body) body->UpdatePhysics(deltaTime);
    }
    
    std::vector<CollisionManifold> manifolds;
    DetectCollisions(manifolds);
    ResolveCollisions(manifolds);
}

} // namespace engine
