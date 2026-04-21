#include "MagicBlock.h"
#include "Ball.h"
#include "GameSettings.h"
#include <cmath>
#include <algorithm>
#include <iostream>

namespace ArkanoidGame
{
    static std::vector<std::unique_ptr<Block>>* g_blocksPtr = nullptr;

    void SetGlobalBlocksPtr(std::vector<std::unique_ptr<Block>>* blocks)
    {
        g_blocksPtr = blocks;
    }

    MagicBlock::MagicBlock(const sf::Vector2f& position)
        : Block(position)  // ← вызываем конструктор обычного блока
    {
        // Загружаем шрифт
        if (!font.loadFromFile("D:/xyz/HW_03/xyz-cpp-course/ArkanoidGame/Resources/Fonts/Roboto-Black.ttf"))
        {
            font.loadFromFile("Resources/Fonts/Roboto-Black.ttf");
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

        // Рисуем зелёный блок
        sf::RectangleShape rect(sf::Vector2f(SETTINGS.BLOCK_WIDTH, SETTINGS.BLOCK_HEIGHT));
        rect.setOrigin(SETTINGS.BLOCK_WIDTH / 2.f, SETTINGS.BLOCK_HEIGHT / 2.f);
        rect.setPosition(GetPosition());
        rect.setFillColor(sf::Color::Green);
        rect.setOutlineColor(sf::Color::Yellow);
        rect.setOutlineThickness(3);
        window.draw(rect);

        // Рисуем счётчик ударов
        window.draw(hitCounterText);
    }

    void MagicBlock::ExplodeNeighbors()
    {
        if (!g_blocksPtr) return;

        sf::Vector2f myPos = GetPosition();
        float blockW = SETTINGS.BLOCK_WIDTH;
        float blockH = SETTINGS.BLOCK_HEIGHT;
        float padding = SETTINGS.BLOCK_PADDING;

        float neighborDistX = blockW + padding;
        float neighborDistY = blockH + padding;

        std::vector<sf::Vector2f> neighborPositions = {
            {myPos.x, myPos.y - neighborDistY},  // верх
            {myPos.x, myPos.y + neighborDistY},  // низ
            {myPos.x - neighborDistX, myPos.y},  // лево
            {myPos.x + neighborDistX, myPos.y}   // право
        };

        for (auto& block : *g_blocksPtr)
        {
            if (!block->IsActive()) continue;

            sf::Vector2f blockPos = block->GetPosition();

            for (const auto& neighborPos : neighborPositions)
            {
                float dx = std::abs(blockPos.x - neighborPos.x);
                float dy = std::abs(blockPos.y - neighborPos.y);

                if (dx < 5.0f && dy < 5.0f)
                {
                    // Уничтожаем соседний блок
                    block->OnHit();
                    std::cout << "[MagicBlock] Exploded neighbor at (" << blockPos.x << ", " << blockPos.y << ")\n";
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

        // Выталкивание мяча
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
            if (leftOverlap < rightOverlap)
                correction.x = -leftOverlap;
            else
                correction.x = rightOverlap;
            ball.BounceX();
        }
        else
        {
            if (topOverlap < bottomOverlap)
                correction.y = -topOverlap;
            else
                correction.y = bottomOverlap;
            ball.BounceY();
        }
        ball.SetPosition(ballPos + correction);

        // Эффект от удара по горизонтали
        sf::Vector2f vel = ball.GetVelocity();
        float hitOffset = (ball.GetPosition().x - GetPosition().x) / (SETTINGS.BLOCK_WIDTH / 2.0f);
        vel.x += hitOffset * 70.0f;

        float currentSpeed = std::sqrt(vel.x * vel.x + vel.y * vel.y);
        if (currentSpeed > 0.0f)
        {
            float targetSpeed = SETTINGS.BALL_SPEED;
            vel.x = (vel.x / currentSpeed) * targetSpeed;
            vel.y = (vel.y / currentSpeed) * targetSpeed;
        }
        ball.SetVelocity(vel);

        // Уменьшаем счётчик
        hitsRemaining--;
        UpdateHitCounterText();

        if (hitsRemaining <= 0)
        {
            ExplodeNeighbors();
            active_ = false;  // блок разрушен
            return true;      // сообщаем, что блок уничтожен (начисляем очки)
        }

        return false;  // блок не уничтожен, очки не начисляем
    }

    void MagicBlock::OnHit()
    {
        // Не нужно, логика в CheckCollision
    }
}
