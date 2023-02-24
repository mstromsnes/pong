#ifndef PADDLE_H
#define PADDLE_H
#include "collider.h"
#include "renderable.h"
#include "stage.h"

#include <memory>
enum class PaddleDirection { Up = 0, Down = 1 };

class Paddle : public Collider, public Renderable
{
  public:
    constexpr Paddle()
        : Collider(),
          Renderable(), placement{0, 0, 0, 0}, prev_placement{placement} {};
    constexpr Paddle(Extent pos)
        : Collider(), Renderable(), placement{pos}, prev_placement{pos} {};
    void move(PaddleDirection dir, int speed, const Stage& stage);

    [[nodiscard]] const Position& getPosition() const noexcept
    {
        return placement.pos;
    };
    [[nodiscard]] const Size& getSize() const noexcept
    {
        return placement.size;
    };

    [[nodiscard]] const Extent& getHitbox() const noexcept override
    {
        return placement;
    };
    void collide(CollisionType) noexcept override{};

    void render(Stage&) override;
    void clear(Stage&) override;
    bool collided() { return collision; };

  private:
    bool collision = false;
    Extent placement;
    Extent prev_placement;
};

#endif // PADDLE_H
