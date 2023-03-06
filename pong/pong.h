#ifndef PONG_H
#define PONG_H
#include "ball.h"
#include "collider.h"
#include "input.h"
#include "paddle.h"
#include "renderable.h"
#include "stage.h"

#include <array>
#include <numbers>
#include <vector>

template <typename T> struct Paddles
{
    Paddle<T> left;
    Paddle<T> right;
};
template <typename T> struct Walls
{
    Paddle<T> top;
    Paddle<T> bottom;
};

class Pong
{
  public:
    constexpr Pong()
        : m_paddles{Paddle<int>{leftPaddleRectangle()},
                    Paddle<int>{rightPaddleRectangle()}},
          m_walls{Paddle<int>{topWallRectangle()},
                  Paddle<int>{
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
    };

    Paddles<int> getPaddles() { return m_paddles; };

    void keyPress(KeyPress);
    void keyRelease(KeyPress);

    void eventLoop();
    void renderLoop();
    Stage& getStage() { return m_stage; };

  private:
    constexpr Rectangle<int> leftPaddleRectangle() const
    {
        const int LEFT_SIDE = 0;
        const int BOTTOM_SIDE = m_stage.bounds().height;
        const int CENTERED_PADDLE_Y_COORDINATE =
            BOTTOM_SIDE / 2 - PADDLE_HEIGHT / 2;
        constexpr const int PADDLE_X_COORDINATE_MARGIN = 50;
        return Rectangle<int>{LEFT_SIDE + PADDLE_X_COORDINATE_MARGIN,
                              CENTERED_PADDLE_Y_COORDINATE, PADDLE_WIDTH,
                              PADDLE_HEIGHT};
    }
    constexpr Rectangle<int> rightPaddleRectangle() const
    {
        const int RIGHT_SIDE = m_stage.bounds().width;
        const int BOTTOM_SIDE = m_stage.bounds().height;
        const int CENTERED_PADDLE_Y_COORDINATE =
            BOTTOM_SIDE / 2 - PADDLE_HEIGHT / 2;
        constexpr const int PADDLE_X_COORDINATE_MARGIN = 50;
        auto rec = Rectangle<int>{
            RIGHT_SIDE - PADDLE_X_COORDINATE_MARGIN - PADDLE_WIDTH,
            CENTERED_PADDLE_Y_COORDINATE, PADDLE_WIDTH, PADDLE_HEIGHT};
        rec.rotate(std::numbers::pi / 4);
        return rec;
    }
    constexpr Rectangle<int> topWallRectangle() const
    {
        const int RIGHT_SIDE = m_stage.bounds().width;
        return Rectangle{0, 0, RIGHT_SIDE, WALL_HEIGHT};
    }
    constexpr Rectangle<int> bottomWallRectangle() const
    {
        const int BOTTOM_SIDE = m_stage.bounds().height;
        const int RIGHT_SIDE = m_stage.bounds().width;
        return Rectangle{0, BOTTOM_SIDE - WALL_HEIGHT, RIGHT_SIDE, WALL_HEIGHT};
    }
    constexpr Rectangle<int> ballRectangle() const
    {
        const int RIGHT_SIDE = m_stage.bounds().width;
        const int BOTTOM_SIDE = m_stage.bounds().height;
        return Rectangle<int>{RIGHT_SIDE / 2 - BALL_SIDE_LENGTH / 2,
                              BOTTOM_SIDE / 2 - BALL_SIDE_LENGTH / 2,
                              BALL_SIDE_LENGTH, BALL_SIDE_LENGTH};
    }
    constexpr std::array<Ball<int>, 1> makeBalls()
    {
        Speed<float> ballInitialSpeed{1.5f, 1};

        return std::array{Ball<int>{ballRectangle(), ballInitialSpeed}};
    }
    constexpr void drawHitboxNormals(Collider<int>&);
    Stage m_stage;
    Paddles<int> m_paddles;
    Walls<int> m_walls;
    static constexpr const int PADDLE_HEIGHT = 30;
    static constexpr const int PADDLE_WIDTH = 6;
    static constexpr const int BALL_SIDE_LENGTH = 6;
    static constexpr const int WALL_HEIGHT = 5;
    std::array<bool, static_cast<size_t>(KeyPress::Num_Keys)> m_keysPressed{
        false};
    std::array<Ball<int>, 1> m_balls;
    std::vector<std::reference_wrapper<Renderable>> m_renderables;
    bool m_debugMode = false;

    void moveBalls();
    void movePaddles(KeyPress);
    void moveBall(KeyPress);
    void collision();
    void pollKeys();
    constexpr void drawNormals();
    constexpr void updatePixmap();
};

#endif // PONG_H
