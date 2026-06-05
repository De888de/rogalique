#pragma once
#include "../Core/Component.h"
#include "Vector2.h"

namespace engine {

    class Collider;
    class TransformComponent;

    class RigidbodyComponent : public Component {
    public:
        RigidbodyComponent();

        void UpdatePhysics(float deltaTime);
        void ApplyForce(Vector2 force);
        void ApplyImpulse(Vector2 impulse);
        void ClearForces();

        // Velocity
        void SetLinearVelocity(Vector2 velocity) { linearVelocity = velocity; }
        Vector2 GetLinearVelocity() const { return linearVelocity; }

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

        Vector2 linearVelocity = Vector2::Zero;
        float angularVelocity = 0.0f;
        Vector2 forceAccumulator = Vector2::Zero;
        float torqueAccumulator = 0.0f;

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