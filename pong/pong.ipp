#include "geometry/constants.h"

#include <algorithm>

constexpr Pong::Pong()
    : m_paddles{Paddle<pos_type>{leftPaddleRectangle()}, Paddle<pos_type>{rightPaddleRectangle()}},
      m_walls{Paddle<pos_type>{topWallRectangle()},
              Paddle<pos_type>{
                  bottomWallRectangle(),
              }},
      m_balls{makeBalls()}
{
    m_renderables.push_back(m_paddles.left);

    m_renderables.push_back(m_paddles.right);

    m_renderables.push_back(m_balls[0]);

    m_renderables.push_back(m_walls.top);

    m_renderables.push_back(m_walls.bottom);
    updatePixmap();
}

constexpr void Pong::eventLoop()
{
    pollKeys();
    if (!m_debugMode)
        moveBalls();
    collision();
}

constexpr void Pong::renderLoop()
{
    updatePixmap();
    if (m_debugMode)
        drawNormals();
}

constexpr void Pong::moveBalls()
{
    for (auto& ball : m_balls)
    {
        ball.move(m_stage);
    }
}

constexpr void Pong::collision()
{
    for (auto& ball : m_balls)
    {
        {
            auto [overlap, minimumTranslationVector] = Collider<pos_type>::overlap<4, 4>(
                ball.getHitbox(), m_paddles.left.getHitbox(), ball.getSpeed().velocity);
            if (overlap)
            {
                ball.collide(minimumTranslationVector);
                m_paddles.left.collide(minimumTranslationVector);
            }
        }
        {
            auto [overlap, minimumTranslationVector] = Collider<pos_type>::overlap<4, 4>(
                ball.getHitbox(), m_paddles.right.getHitbox(), ball.getSpeed().velocity);
            if (overlap)
            {
                ball.collide(minimumTranslationVector);
                m_paddles.right.collide(minimumTranslationVector);
            }
        }
        {
            auto [overlap, minimumTranslationVector] =
                Collider<pos_type>::overlap<4, 4>(ball.getHitbox(), m_walls.top.getHitbox(), ball.getSpeed().velocity);
            if (overlap)
            {
                ball.collide(minimumTranslationVector);
                m_walls.top.collide(minimumTranslationVector);
            }
        }
        {
            auto [overlap, minimumTranslationVector] = Collider<pos_type>::overlap<4, 4>(
                ball.getHitbox(), m_walls.bottom.getHitbox(), ball.getSpeed().velocity);
            if (overlap)
            {
                ball.collide(minimumTranslationVector);
                m_walls.bottom.collide(minimumTranslationVector);
            }
        }
    }
}

constexpr void Pong::pollKeys()
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

constexpr void Pong::movePaddles(KeyPress press)
{
    const int playable_area_top = 0 + m_walls.top.getHitbox().height();
    const int playable_area_bottom = m_stage.bounds().height - m_walls.bottom.getHitbox().height();
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

constexpr void Pong::moveBall(KeyPress press)
{
    switch (press)
    {
    case KeyPress::i:
        m_balls[0].move_custom(-constants::YUnitVector<pos_type>);
        break;
    case KeyPress::j:
        m_balls[0].move_custom(-constants::XUnitVector<pos_type>);
        break;
    case KeyPress::k:
        m_balls[0].move_custom(constants::YUnitVector<pos_type>);
        break;
    case KeyPress::l:
        m_balls[0].move_custom(constants::XUnitVector<pos_type>);
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

constexpr void Pong::drawHitboxNormals(Collider<pos_type>& collider)
{
    auto vert = collider.getHitbox().verticesCW();
    auto lines = std::array<Line<pos_type>, 4>{Line<pos_type>(vert[0], vert[3]), Line<pos_type>(vert[1], vert[0]),
                                               Line<pos_type>(vert[2], vert[1]), Line<pos_type>(vert[3], vert[2])};
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

constexpr void Pong::keyPress(KeyPress press)
{
    m_keysPressed[static_cast<int>(press)] = true;
    if (press == KeyPress::DebugMode)
        m_debugMode = !m_debugMode;
}

constexpr void Pong::keyRelease(KeyPress press)
{
    m_keysPressed[static_cast<int>(press)] = false;
}

constexpr auto Pong::leftPaddleRectangle() const -> Rectangle<int>
{
    const int LEFT_SIDE = 0;
    const int BOTTOM_SIDE = m_stage.bounds().height;
    const int CENTERED_PADDLE_Y_COORDINATE = BOTTOM_SIDE / 2 - PADDLE_HEIGHT / 2;
    constexpr const int PADDLE_X_COORDINATE_MARGIN = 50;
    return Rectangle<int>{LEFT_SIDE + PADDLE_X_COORDINATE_MARGIN, CENTERED_PADDLE_Y_COORDINATE, PADDLE_WIDTH,
                          PADDLE_HEIGHT};
}

constexpr auto Pong::rightPaddleRectangle() const -> Rectangle<int>
{
    const int RIGHT_SIDE = m_stage.bounds().width;
    const int BOTTOM_SIDE = m_stage.bounds().height;
    const int CENTERED_PADDLE_Y_COORDINATE = BOTTOM_SIDE / 2 - PADDLE_HEIGHT / 2;
    constexpr const int PADDLE_X_COORDINATE_MARGIN = 50;
    auto rec = Rectangle<int>{RIGHT_SIDE - PADDLE_X_COORDINATE_MARGIN - PADDLE_WIDTH, CENTERED_PADDLE_Y_COORDINATE,
                              PADDLE_WIDTH, PADDLE_HEIGHT};
    // rec.rotate(std::numbers::pi / 4);
    return rec;
}

constexpr auto Pong::topWallRectangle() const -> Rectangle<int>
{
    const int RIGHT_SIDE = m_stage.bounds().width;
    return Rectangle{0, 0, RIGHT_SIDE, WALL_HEIGHT};
}

constexpr auto Pong::bottomWallRectangle() const -> Rectangle<int>
{
    const int BOTTOM_SIDE = m_stage.bounds().height;
    const int RIGHT_SIDE = m_stage.bounds().width;
    return Rectangle{0, BOTTOM_SIDE - WALL_HEIGHT, RIGHT_SIDE, WALL_HEIGHT};
}

constexpr auto Pong::ballRectangle() const -> Rectangle<int>
{
    const int RIGHT_SIDE = m_stage.bounds().width;
    const int BOTTOM_SIDE = m_stage.bounds().height;
    return Rectangle<int>{RIGHT_SIDE / 2 - BALL_SIDE_LENGTH / 2, BOTTOM_SIDE / 2 - BALL_SIDE_LENGTH / 2,
                          BALL_SIDE_LENGTH, BALL_SIDE_LENGTH};
}

constexpr auto Pong::makeBalls() const -> std::array<Ball<pos_type>, 1>
{
    Speed<double> ballInitialSpeed{1.5f, 1};

    return std::array{Ball<pos_type>{ballRectangle(), ballInitialSpeed}};
}
