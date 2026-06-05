#pragma once
#include "../PhysicsMaterial.h"
#include "../../Core/Component.h"
#include "Vector2.h"                    

namespace engine {

    class TransformComponent;

    enum class ColliderType {
        Circle,
        Box,
        Polygon
    };

    class Collider : public Component {
    public:
        ColliderType type;
        PhysicsMaterial material;
        bool isTrigger = false;

        Collider(ColliderType t) : type(t) {}
        virtual ~Collider() = default;

        virtual bool CheckCollision(Collider* other, Vector2& normal, float& penetration) = 0;
        virtual Vector2 GetCenter() const;

    protected:
        TransformComponent* GetTransform() const;
    };

} 