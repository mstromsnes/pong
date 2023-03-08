#include "pong.h"

#include "geometry/constants.h"

#include <algorithm>

void Pong::eventLoop()
{
    pollKeys();
    if (!m_debugMode)
        moveBalls();
    collision();
}

void Pong::renderLoop()
{
    updatePixmap();
    if (m_debugMode)
        drawNormals();
}

void Pong::moveBalls()
{
    for (auto& ball : m_balls)
    {
        ball.move(m_stage);
    }
}

void Pong::collision()
{
    for (auto& ball : m_balls)
    {
        {
            auto [overlap, minimumTranslationVector] =
                Collider<int>::overlap<4, 4>(ball.getHitbox(),
                                             m_paddles.left.getHitbox(),
                                             ball.getSpeed().velocity);
            if (overlap)
            {
                ball.collide(minimumTranslationVector);
                m_paddles.left.collide(minimumTranslationVector);
            }
        }
        {
            auto [overlap, minimumTranslationVector] =
                Collider<int>::overlap<4, 4>(ball.getHitbox(),
                                             m_paddles.right.getHitbox(),
                                             ball.getSpeed().velocity);
            if (overlap)
            {
                ball.collide(minimumTranslationVector);
                m_paddles.right.collide(minimumTranslationVector);
            }
        }
        {
            auto [overlap, minimumTranslationVector] =
                Collider<int>::overlap<4, 4>(ball.getHitbox(),
                                             m_walls.top.getHitbox(),
                                             ball.getSpeed().velocity);
            if (overlap)
            {
                ball.collide(minimumTranslationVector);
                m_walls.top.collide(minimumTranslationVector);
            }
        }
        {
            auto [overlap, minimumTranslationVector] =
                Collider<int>::overlap<4, 4>(ball.getHitbox(),
                                             m_walls.bottom.getHitbox(),
                                             ball.getSpeed().velocity);
            if (overlap)
            {
                ball.collide(minimumTranslationVector);
                m_walls.bottom.collide(minimumTranslationVector);
            }
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
            case KeyPress::P1Left:
            case KeyPress::P1Right:
            case KeyPress::P2Left:
            case KeyPress::P2Right:
                if (m_debugMode)
                    movePaddles(KeyPress(i));
                break;
            case KeyPress::i:
            case KeyPress::j:
            case KeyPress::k:
            case KeyPress::l:
                if (m_debugMode)
                    moveBall(KeyPress(i));
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
    const int playable_area_left = 0;
    const int playable_area_right = m_stage.bounds().width;
    const int speed = 3;
    switch (press)
    {
    case KeyPress::P1Down:
        if (m_paddles.left.getHitbox().bottom() < playable_area_bottom)
            m_paddles.left.move(PaddleDirection::Down, speed);
        break;
    case KeyPress::P1Up:
        if (m_paddles.left.getHitbox().top() > playable_area_top)
            m_paddles.left.move(PaddleDirection::Up, speed);
        break;
    case KeyPress::P1Left:
        if (m_paddles.left.getHitbox().left() > playable_area_left)
            m_paddles.left.move(PaddleDirection::Left, speed);
        break;
    case KeyPress::P1Right:
        if (m_paddles.left.getHitbox().right() < playable_area_right)
            m_paddles.left.move(PaddleDirection::Right, speed);
        break;
    case KeyPress::P2Down:
        if (m_paddles.right.getHitbox().bottom() < playable_area_bottom)
            m_paddles.right.move(PaddleDirection::Down, speed);
        break;
    case KeyPress::P2Up:
        if (m_paddles.right.getHitbox().top() > playable_area_top)
            m_paddles.right.move(PaddleDirection::Up, speed);
        break;
    case KeyPress::P2Left:
        if (m_paddles.right.getHitbox().left() > playable_area_left)
            m_paddles.right.move(PaddleDirection::Left, speed);
        break;
    case KeyPress::P2Right:
        if (m_paddles.right.getHitbox().right() < playable_area_right)
            m_paddles.right.move(PaddleDirection::Right, speed);
        break;
    default:
        break;
    }
}
void Pong::moveBall(KeyPress press)
{
    switch (press)
    {
    case KeyPress::i:
        m_balls[0].move_custom(-constants::YUnitVector<double>);
        break;
    case KeyPress::j:
        m_balls[0].move_custom(-constants::XUnitVector<double>);
        break;
    case KeyPress::k:
        m_balls[0].move_custom(constants::YUnitVector<double>);
        break;
    case KeyPress::l:
        m_balls[0].move_custom(constants::XUnitVector<double>);
        break;
    default:
        break;
    }
}
constexpr void Pong::updatePixmap()
{
    m_stage.clear();
    for (Renderable& renderable : m_renderables)
    {
        renderable.render(m_stage, m_debugMode);
    }
}
constexpr void Pong::drawHitboxNormals(Collider<int>& collider)
{
    auto vert = collider.getHitbox().verticesCW();
    auto lines = std::array<Line<int>, 4>{
        Line<int>(vert[0], vert[3]), Line<int>(vert[1], vert[0]),
        Line<int>(vert[2], vert[1]), Line<int>(vert[3], vert[2])};
    for (auto line : lines)
    {
        m_stage.drawNormal(line, Color{0x00, 0xff, 0xff});
    }
}
constexpr void Pong::drawNormals()
{
    drawHitboxNormals(m_balls[0]);
    drawHitboxNormals(m_paddles.left);
    drawHitboxNormals(m_paddles.right);
    drawHitboxNormals(m_walls.top);
    drawHitboxNormals(m_walls.bottom);
}
void Pong::keyPress(KeyPress press)
{
    m_keysPressed[static_cast<int>(press)] = true;
    if (press == KeyPress::DebugMode)
        m_debugMode = !m_debugMode;
}

void Pong::keyRelease(KeyPress press)
{
    m_keysPressed[static_cast<int>(press)] = false;
}
