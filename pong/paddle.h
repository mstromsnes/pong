#ifndef PADDLE_H
#define PADDLE_H
#include "collider.h"
#include "renderable.h"
#include "stage.h"

#include <memory>
enum class PaddleDirection { Up = 0, Down = 1 };

template <typename T> class Paddle : public Collider<T>, public Renderable
{
  public:
    constexpr Paddle()
        : Collider<T>(),
          Renderable(), placement{0, 0, 0, 0}, prev_placement{placement} {};
    constexpr Paddle(Rectangle<T> pos)
        : Collider<T>(), Renderable(), placement{pos}, prev_placement{pos} {};
    void move(PaddleDirection dir, T speed, const Stage& stage)
    {
        switch (dir)
        {
        case PaddleDirection::Down:
            if (placement.bottomLeft().y < stage.bounds().bottomLeft().y)
            {
                placement.translate(Vector2D<float>(0, speed));
                collision = false;
            }
            else
                collision = true;
            break;
        case PaddleDirection::Up:
            if (placement.topLeft().y > stage.bounds().topLeft().y)
            {
                placement.translate(Vector2D<float>(0, -speed));
                collision = false;
            }
            else
                collision = true;
            break;
        default:
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

    void render(Stage& stage) override
    {
        clear(stage);
        prev_placement = placement;
        stage.fill(placement, Color(0x00, 0x00, 0xff));
    }
    void clear(Stage& stage) override { stage.fill(prev_placement, 0x0); }
    bool collided() { return collision; };

  private:
    bool collision = false;
    Rectangle<T> placement;
    Rectangle<T> prev_placement;
};

#endif // PADDLE_H
