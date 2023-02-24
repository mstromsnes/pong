#ifndef PONG_H
#define PONG_H
#include "ball.h"
#include "collider.h"
#include "input.h"
#include "paddle.h"
#include "renderable.h"
#include "stage.h"

#include <array>
#include <vector>

struct Paddles
{
    Paddle left;
    Paddle right;
};
struct Walls
{
    Paddle top;
    Paddle bottom;
};

class Pong
{
  public:
    constexpr Pong()
    {
        const int CENTERED_PADDLE_Y_COORDINATE =
            m_stage.bounds().size.height / 2 - PADDLE_HEIGHT / 2;
        constexpr const int PADDLE_X_COORDINATE_MARGIN = 10;
        Extent paddleExtent{
            m_stage.bounds().left() + PADDLE_X_COORDINATE_MARGIN,
            CENTERED_PADDLE_Y_COORDINATE, PADDLE_WIDTH, PADDLE_HEIGHT};
        m_paddles.left = Paddle(paddleExtent);
        m_renderables.push_back(m_paddles.left);

        Extent paddle2Extent{m_stage.bounds().right() -
                                 PADDLE_X_COORDINATE_MARGIN - PADDLE_WIDTH,
                             CENTERED_PADDLE_Y_COORDINATE, PADDLE_WIDTH,
                             PADDLE_HEIGHT};
        m_paddles.right = Paddle(paddle2Extent);
        m_renderables.push_back(m_paddles.right);

        Extent ballExtent{
            (m_stage.bounds().left() + m_stage.bounds().right()) / 2,
            (m_stage.bounds().top() + m_stage.bounds().bottom()) / 2,
            BALL_SIDE_LENGTH, BALL_SIDE_LENGTH};
        Speed ballInitialSpeed{2, 1};
        m_balls[0].setPlacement(ballExtent);
        m_balls[0].setSpeed(ballInitialSpeed);
        m_renderables.push_back(m_balls[0]);

        Extent topWallExtent{0, 0, m_stage.bounds().size.width, 5};
        m_walls.top = Paddle(topWallExtent);
        m_renderables.push_back(m_walls.top);

        Extent bottomWallExtent{0, m_stage.bounds().bottom() - 5,
                                m_stage.bounds().size.width, 5};
        m_walls.bottom = Paddle(bottomWallExtent);
        m_renderables.push_back(m_walls.bottom);
    };

    Paddles getPaddles() { return m_paddles; };

    void keyPress(KeyPress);
    void keyRelease(KeyPress);

    void eventLoop();
    Stage& getStage() { return m_stage; };

  private:
    // RenderStrategy m_renderer;
    // KeyInputStrategy m_inputhandler;
    Stage m_stage;
    Paddles m_paddles;
    Walls m_walls;
    static constexpr const int PADDLE_HEIGHT = 30;
    static constexpr const int PADDLE_WIDTH = 6;
    static constexpr const int BALL_SIDE_LENGTH = 6;
    std::array<bool, static_cast<size_t>(KeyPress::Num_Keys)> m_keysPressed{
        false};
    std::array<Ball, 1> m_balls;
    std::vector<std::reference_wrapper<Renderable>> m_renderables;

    void moveBalls();
    void movePaddles(KeyPress);
    void collision();
    void pollKeys();
    void updatePixmap();
};

#endif // PONG_H
