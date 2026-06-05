#pragma once
#include "../Core/Component.h"
#include <SFML/System/Vector2.hpp>

namespace engine {

class Collider;
class TransformComponent;

class RigidbodyComponent : public Component {
public:
    RigidbodyComponent();
    
    void UpdatePhysics(float deltaTime);
    void ApplyForce(sf::Vector2f force);
    void ApplyImpulse(sf::Vector2f impulse);
    void ClearForces();
    
    // Velocity
    void SetLinearVelocity(sf::Vector2f velocity) { linearVelocity = velocity; }
    sf::Vector2f GetLinearVelocity() const { return linearVelocity; }
    
    void SetAngularVelocity(float velocity) { angularVelocity = velocity; }
    float GetAngularVelocity() const { return angularVelocity; }
    
    // Mass
    void SetMass(float mass);
    float GetMass() const { return mass; }
    float GetInvMass() const { return invMass; }
    
    // Damping
    void SetLinearDamping(float damping) { linearDamping = damping; }
    void SetAngularDamping(float damping) { angularDamping = damping; }
    
    // Kinematic
    void SetKinematic(bool kinematic) { isKinematic = kinematic; }
    bool IsKinematic() const { return isKinematic; }
    
    void SetGravityScale(float scale) { gravityScale = scale; }
    float GetGravityScale() const { return gravityScale; }
    
    void AttachCollider(Collider* col) { collider = col; }
    Collider* GetCollider() const { return collider; }
    
private:
    TransformComponent* transform = nullptr;
    Collider* collider = nullptr;
    
    sf::Vector2f linearVelocity = {0, 0};
    float angularVelocity = 0;
    sf::Vector2f forceAccumulator = {0, 0};
    float torqueAccumulator = 0;
    
    float mass = 1.0f;
    float invMass = 1.0f;
    float inertia = 1.0f;
    float invInertia = 1.0f;
    
    float linearDamping = 0.01f;
    float angularDamping = 0.01f;
    
    bool isKinematic = false;
    float gravityScale = 1.0f;
    
    void UpdateInverseValues();
    void ApplyForces();
    void Integrate(float deltaTime);
    TransformComponent* GetTransform() const;
};

} // namespace engine
