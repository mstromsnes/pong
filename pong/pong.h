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

template <typename T>
struct Paddles
{
    Paddle<T> left;
    Paddle<T> right;
};
template <typename T>
struct Walls
{
    Paddle<T> top;
    Paddle<T> bottom;
};

class Pong
{
  public:
    constexpr Pong();
    auto getPaddles() -> Paddles<int> { return m_paddles; };

    constexpr void keyPress(KeyPress);
    constexpr void keyRelease(KeyPress);

    constexpr void eventLoop();
    constexpr void renderLoop();
    constexpr Stage& getStage() { return m_stage; };

  private:
    Stage m_stage;
    Paddles<int> m_paddles;
    Walls<int> m_walls;
    static constexpr const int PADDLE_HEIGHT = 30;
    static constexpr const int PADDLE_WIDTH = 6;
    static constexpr const int BALL_SIDE_LENGTH = 6;
    static constexpr const int WALL_HEIGHT = 5;
    std::array<bool, static_cast<size_t>(KeyPress::Num_Keys)> m_keysPressed{false};
    std::array<Ball<int>, 1> m_balls;
    std::vector<std::reference_wrapper<Renderable>> m_renderables;
    bool m_debugMode = false;

    constexpr auto leftPaddleRectangle() const -> Rectangle<int>;
    constexpr auto rightPaddleRectangle() const -> Rectangle<int>;
    constexpr auto topWallRectangle() const -> Rectangle<int>;
    constexpr auto bottomWallRectangle() const -> Rectangle<int>;
    constexpr auto ballRectangle() const -> Rectangle<int>;
    constexpr auto makeBalls() const -> std::array<Ball<int>, 1>;

    constexpr void drawHitboxNormals(Collider<int>&);
    constexpr void moveBalls();
    constexpr void movePaddles(KeyPress);
    constexpr void moveBall(KeyPress);
    constexpr void collision();
    constexpr void pollKeys();
    constexpr void drawNormals();
    constexpr void updatePixmap();
};
#include "pong.ipp"

#endif // PONG_H
