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
    constexpr Paddle() : placement{0, 0, 0, 0} {};
    constexpr Paddle(Rectangle<T> pos) : placement{pos} {};

    constexpr void move(PaddleDirection dir, T speed)
    {
        switch (dir)
        {
        case PaddleDirection::Down:
            placement.translate(Vector2D<float>(0, speed));
            collision = false;
            break;
        case PaddleDirection::Up:
            placement.translate(Vector2D<float>(0, -speed));
            collision = false;
            break;
        case PaddleDirection::Left:
            placement.translate(Vector2D<float>(-speed, 0));
            collision = false;
            break;
        case PaddleDirection::Right:
            placement.translate(Vector2D<float>(speed, 0));
            collision = false;
            break;
        }
    }

    [[nodiscard]] const Position<T>& getPosition() const noexcept
    {
        return placement.pos();
    };
    [[nodiscard]] const Size<T>& getSize() const noexcept
    {
        return placement.size();
    };

    [[nodiscard]] const Rectangle<T>& getHitbox() const noexcept override
    {
        return placement;
    };
    void collide(CollisionType) noexcept override{};

    constexpr void render(Stage& stage) override
    {
        stage.fillRectangle(placement, Color(0x00, 0x00, 0xff));
    }
    constexpr bool collided() { return collision; };

  private:
    bool collision = false;
    Rectangle<T> placement;
};

#endif // PADDLE_H
