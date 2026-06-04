#pragma once
#include "Component.h"

namespace rogalique
{
    // Компонент-маркер для блоков (стен)
    class BlockComponent : public Component
    {
    public:
        BlockComponent(RogaliqueGameObject* owner);
        void Update(float deltaTime) override;
        void Render(sf::RenderWindow& window) override;
        
        bool IsMagic() const { return m_isMagic; }
        void SetMagic(bool magic) { m_isMagic = magic; }
        
    private:
        bool m_isMagic = false;
    };
}
