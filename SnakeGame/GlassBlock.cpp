#include "GlassBlock.h"
#include "Ball.h"
#include "GameSettings.h"
#include <cmath>

namespace ArkanoidGame
{
    GlassBlock::GlassBlock(const sf::Vector2f& position)
        : Block(position)
    {
        active_ = true;
    }

    void GlassBlock::Draw(sf::RenderWindow& window)
    {
        if (!active_) return;

        sf::RectangleShape rect(sf::Vector2f(SETTINGS.BLOCK_WIDTH, SETTINGS.BLOCK_HEIGHT));
        rect.setOrigin(SETTINGS.BLOCK_WIDTH / 2.f, SETTINGS.BLOCK_HEIGHT / 2.f);
        rect.setPosition(GetPosition());
        rect.setFillColor(glassColor);
        rect.setOutlineColor(outlineColor);
        rect.setOutlineThickness(2);

        // Делаем блок полупрозрачным (10% непрозрачности = 25 из 255)
        sf::Color transparentColor = glassColor;
        transparentColor.a = 25;  // 10% от 255 ≈ 25
        rect.setFillColor(transparentColor);

        window.draw(rect);
    }

    bool GlassBlock::CheckCollision(Ball& ball)
    {
        if (!active_) return false;

        sf::FloatRect ballRect = ball.GetRect();
        sf::FloatRect blockRect = GetCollisionRect();

        if (!ballRect.intersects(blockRect))
            return false;

        // ❗ ВАЖНО: НЕТ ВЫТАЛКИВАНИЯ МЯЧА
        // ❗ НЕТ ИЗМЕНЕНИЯ СКОРОСТИ
        // Мяч просто пролетает сквозь блок

        // Уничтожаем блок
        OnHit();

        // Возвращаем true, чтобы начислить очки
        return true;
    }

    void GlassBlock::OnHit()
    {
        active_ = false;
        isDestroyed = true;
    }
}
