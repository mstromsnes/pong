#include "pong.h"

#include <algorithm>

void Pong::eventLoop()
{
    moveBalls();
    collision();
    pollKeys();
    updatePixmap();
}

void Pong::moveBalls()
{
    for (auto& ball : m_balls)
    {
        ball.move();
    }
}

void Pong::collision()
{
    for (auto& ball : m_balls)
    {
        if (Collider<int>::overlap(ball.getHitbox(),
                                   m_paddles.left.getHitbox()) ||
            Collider<int>::overlap(ball.getHitbox(),
                                   m_paddles.right.getHitbox()))
        {
            ball.collide(CollisionType::Vertical);
        }
        else if (Collider<int>::overlap(ball.getHitbox(),
                                        m_walls.top.getHitbox()) ||
                 Collider<int>::overlap(ball.getHitbox(),
                                        m_walls.bottom.getHitbox()))
        {
            ball.collide(CollisionType::Horizontal);
        }
    }
}
void Pong::pollKeys()
{
    for (size_t i = 0; i < m_keysPressed.size(); i++)
    {
        const auto& pressed = m_keysPressed[i];
        if (pressed)
        {
            switch (KeyPress(i))
            {
            case KeyPress::P1Down:
            case KeyPress::P1Up:
            case KeyPress::P2Down:
            case KeyPress::P2Up:
                movePaddles(KeyPress(i));
                break;
            case KeyPress::Enter:
                m_balls[0].reset();
                break;
            case KeyPress::Escape:
                break;
            default:
                break;
            }
        }
    }
}

void Pong::movePaddles(KeyPress press)
{
    const int playable_area_top = 0 + m_walls.top.getHitbox().height();
    const int playable_area_bottom =
        m_stage.bounds().height - m_walls.bottom.getHitbox().height();
    switch (press)
    {
    case KeyPress::P1Down:
        if (m_paddles.left.getHitbox().bottom() < playable_area_bottom)
            m_paddles.left.move(PaddleDirection::Down, 2, m_stage);
        break;
    case KeyPress::P1Up:
        if (m_paddles.left.getHitbox().top() > playable_area_top)
            m_paddles.left.move(PaddleDirection::Up, 2, m_stage);
        break;
    case KeyPress::P2Down:
        if (m_paddles.right.getHitbox().bottom() < playable_area_bottom)
            m_paddles.right.move(PaddleDirection::Down, 2, m_stage);
        break;
    case KeyPress::P2Up:
        if (m_paddles.right.getHitbox().top() > playable_area_top)
            m_paddles.right.move(PaddleDirection::Up, 2, m_stage);
        break;
    default:
        break;
    }
}
void Pong::updatePixmap()
{
    for (Renderable& renderable : m_renderables)
    {
        renderable.render(m_stage);
    }
}

void Pong::keyPress(KeyPress press)
{
    m_keysPressed[static_cast<int>(press)] = true;
}

void Pong::keyRelease(KeyPress press)
{
    m_keysPressed[static_cast<int>(press)] = false;
}
