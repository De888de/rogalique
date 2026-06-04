#pragma once
#include "GameWorld.h"

namespace rogalique
{
    class BlockBuilder
    {
    public:
        static RogaliqueGameObject* CreateBlock(float x, float y, float width, float height, bool isMagic = false);
        static void BuildPerimeter(float worldWidth, float worldHeight, float thickness);
        static void BuildRow(float y, float startX, float endX, float blockSize, bool isMagic = false);
    };
}
