#include "MagicBlock.h"
#include "Ball.h"
#include "GameSettings.h"
#include <cmath>
#include <algorithm>
#include <iostream>

namespace ArkanoidGame
{
    // Статическая переменная для взрыва соседей
    static std::vector<std::unique_ptr<Block>>* g_blocksPtr = nullptr;

    void SetGlobalBlocksPtr(std::vector<std::unique_ptr<Block>>* blocks)
    {
        g_blocksPtr = blocks;
    }

    MagicBlock::MagicBlock(const sf::Vector2f& position)
        : Block(position)
        , hitsRemaining(5)
    {
        if (!font.loadFromFile("Resources/Fonts/Roboto-Black.ttf"))
        {
            font.loadFromFile("D:/xyz/HW_03/xyz-cpp-course/ArkanoidGame/Resources/Fonts/Roboto-Black.ttf");
        }

        hitCounterText.setFont(font);
        hitCounterText.setCharacterSize(16);
        hitCounterText.setFillColor(sf::Color::White);
        hitCounterText.setStyle(sf::Text::Bold);
        UpdateHitCounterText();

        active_ = true;
    }

    void MagicBlock::UpdateHitCounterText()
    {
        hitCounterText.setString(std::to_string(hitsRemaining));
        sf::FloatRect blockRect = GetCollisionRect();
        float textX = blockRect.left + blockRect.width - 18.f;
        float textY = blockRect.top + 4.f;
        hitCounterText.setPosition(textX, textY);
    }

    void MagicBlock::Draw(sf::RenderWindow& window)
    {
        if (!active_) return;

        sf::RectangleShape rect(sf::Vector2f(SETTINGS.BLOCK_WIDTH, SETTINGS.BLOCK_HEIGHT));
        rect.setOrigin(SETTINGS.BLOCK_WIDTH / 2.f, SETTINGS.BLOCK_HEIGHT / 2.f);
        rect.setPosition(GetPosition());
        rect.setFillColor(sf::Color::Green);
        rect.setOutlineColor(sf::Color::Yellow);
        rect.setOutlineThickness(3);
        window.draw(rect);
        window.draw(hitCounterText);
    }

    void MagicBlock::ExplodeNeighbors()
    {
        if (!g_blocksPtr) return;

        sf::Vector2f myPos = GetPosition();
        float blockW = SETTINGS.BLOCK_WIDTH;
        float blockH = SETTINGS.BLOCK_HEIGHT;
        float padding = SETTINGS.BLOCK_PADDING;

        std::vector<sf::Vector2f> directions = {
            {0, -1}, {0, 1}, {-1, 0}, {1, 0}
        };

        for (auto& block : *g_blocksPtr)
        {
            if (!block->IsActive() || block.get() == this) continue;

            sf::Vector2f blockPos = block->GetPosition();
            for (const auto& dir : directions)
            {
                float dx = std::abs(blockPos.x - (myPos.x + dir.x * (blockW + padding)));
                float dy = std::abs(blockPos.y - (myPos.y + dir.y * (blockH + padding)));

                if (dx < 10.0f && dy < 10.0f)
                {
                    block->OnHit();
                    std::cout << "[MagicBlock] Exploded neighbor!" << std::endl;
                    break;
                }
            }
        }
    }

    bool MagicBlock::CheckCollision(Ball& ball)
    {
        if (!active_) return false;

        sf::FloatRect ballRect = ball.GetRect();
        sf::FloatRect blockRect = GetCollisionRect();
        if (!ballRect.intersects(blockRect))
            return false;

        // Выталкивание + отскок (аналогично другим блокам)
        sf::Vector2f ballPos = ball.GetPosition();
        sf::Vector2f correction(0.f, 0.f);

        float leftOverlap = ballRect.left + ballRect.width - blockRect.left;
        float rightOverlap = blockRect.left + blockRect.width - ballRect.left;
        float topOverlap = ballRect.top + ballRect.height - blockRect.top;
        float bottomOverlap = blockRect.top + blockRect.height - ballRect.top;

        float minX = std::min(leftOverlap, rightOverlap);
        float minY = std::min(topOverlap, bottomOverlap);

        if (minX < minY)
        {
            correction.x = (leftOverlap < rightOverlap) ? -leftOverlap : rightOverlap;
            ball.BounceX();
        }
        else
        {
            correction.y = (topOverlap < bottomOverlap) ? -topOverlap : bottomOverlap;
            ball.BounceY();
        }
        ball.SetPosition(ballPos + correction);

        // Эффект от удара
        sf::Vector2f vel = ball.GetVelocity();
        float hitOffset = (ball.GetPosition().x - GetPosition().x) / (SETTINGS.BLOCK_WIDTH / 2.0f);
        vel.x += hitOffset * 70.0f;

        float currentSpeed = std::sqrt(vel.x * vel.x + vel.y * vel.y);
        if (currentSpeed > 0.0f)
        {
            float targetSpeed = SETTINGS.BALL_SPEED;
            vel = vel / currentSpeed * targetSpeed;
        }
        ball.SetVelocity(vel);

        // Уменьшаем прочность
        hitsRemaining--;
        UpdateHitCounterText();

        std::cout << "[MagicBlock] Hit! Remaining hits: " << hitsRemaining << std::endl;

        if (hitsRemaining <= 0)
        {
            std::cout << "[MagicBlock] === DESTROYED! Exploding neighbors + OnHit() ===\n";
            ExplodeNeighbors();
            OnHit();
            return true;
        }

        return false;
    }

    void MagicBlock::OnHit()
    {
        std::cout << "[MagicBlock::OnHit] → Calling Block::OnHit()" << std::endl;
        Block::OnHit();   // Здесь диспатч и начисление очков
    }

    int MagicBlock::GetScoreValue() const
    {
        return 50;   // Волшебный блок даёт много очков
    }
}