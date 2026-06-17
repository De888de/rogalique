#pragma once
#include "RogaliqueGameObject.h"

namespace rogalique
{
    class Wall : public RogaliqueGameObject
    {
    public:
        Wall() = default;
        Wall(float x, float y, float width, float height);
        void Update(float deltaTime) override;
    };
}
