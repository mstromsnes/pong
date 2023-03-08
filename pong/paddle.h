#ifndef PADDLE_H
#define PADDLE_H
#include "collider.h"
#include "renderable.h"
#include "stage.h"

#include <memory>
enum class PaddleDirection { Up = 0, Down = 1, Left = 2, Right = 3 };

template <typename T> class Paddle : public Collider<T>, public Renderable
{
  public:
    constexpr Paddle() : m_placement{0, 0, 0, 0} {};
    constexpr Paddle(Rectangle<T> pos) : m_placement{pos} {};

    constexpr void move(PaddleDirection dir, T speed)
    {
        switch (dir)
        {
        case PaddleDirection::Down:
            m_placement.translate(Vector2D<double>(0, speed));
            break;
        case PaddleDirection::Up:
            m_placement.translate(Vector2D<double>(0, -speed));
            break;
        case PaddleDirection::Left:
            m_placement.translate(Vector2D<double>(-speed, 0));
            break;
        case PaddleDirection::Right:
            m_placement.translate(Vector2D<double>(speed, 0));
            break;
        }
    }

    [[nodiscard]] const Position<T>& getPosition() const noexcept
    {
        return m_placement.pos();
    };
    [[nodiscard]] const Size<T>& getSize() const noexcept
    {
        return m_placement.size();
    };

    [[nodiscard]] const Rectangle<T>& getHitbox() const noexcept override
    {
        return m_placement;
    };
    void collide(Vector2D<double>) noexcept override { m_collision = true; };

    constexpr void render(Stage& stage, bool debug = false) override
    {
        auto color = m_collision && debug ? getDebugColor() : getColor();
        stage.fillRectangle(m_placement, color);
        m_collision = false;
    }
    constexpr bool collided() { return m_collision; };

  private:
    bool m_collision = false;
    Rectangle<T> m_placement;
};

#endif // PADDLE_H
