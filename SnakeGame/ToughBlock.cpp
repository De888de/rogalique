#include "ToughBlock.h"
#include "Ball.h"
#include "GameSettings.h"
#include <cmath>
#include <algorithm>

namespace ArkanoidGame
{
    ToughBlock::ToughBlock(const sf::Vector2f& position)
        : Block(position)  // ← вызываем конструктор обычного блока
    {
        // Загружаем шрифт
        if (!font.loadFromFile("D:/xyz/HW_03/xyz-cpp-course/ArkanoidGame/Resources/Fonts/Roboto-Black.ttf"))
        {
            font.loadFromFile("Resources/Fonts/Roboto-Black.ttf");
        }

        hitCounterText.setFont(font);
        hitCounterText.setCharacterSize(18);
        hitCounterText.setFillColor(sf::Color::Black);
        hitCounterText.setStyle(sf::Text::Bold);
        UpdateHitCounterText();

        active_ = true;
    }

    void ToughBlock::UpdateHitCounterText()
    {
        hitCounterText.setString(std::to_string(hitsRemaining));
        sf::FloatRect blockRect = GetCollisionRect();
        float textX = blockRect.left + blockRect.width - 18.f;
        float textY = blockRect.top + 4.f;
        hitCounterText.setPosition(textX, textY);
    }

    void ToughBlock::Draw(sf::RenderWindow& window)
    {
        if (!active_) return;

        // Рисуем красный блок
        sf::RectangleShape rect(sf::Vector2f(SETTINGS.BLOCK_WIDTH, SETTINGS.BLOCK_HEIGHT));
        rect.setOrigin(SETTINGS.BLOCK_WIDTH / 2.f, SETTINGS.BLOCK_HEIGHT / 2.f);
        rect.setPosition(GetPosition());
        rect.setFillColor(sf::Color::Red);
        rect.setOutlineColor(sf::Color::Yellow);
        rect.setOutlineThickness(2);
        window.draw(rect);

        // Рисуем счётчик ударов
        window.draw(hitCounterText);
    }

    bool ToughBlock::CheckCollision(Ball& ball)
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
            active_ = false;  // блок разрушен
            return true;      // сообщаем, что блок уничтожен (начисляем очки)
        }

        return false;  // блок не уничтожен, очки не начисляем
    }

    void ToughBlock::OnHit()
    {
        // Не нужно, логика в CheckCollision
    }
}
