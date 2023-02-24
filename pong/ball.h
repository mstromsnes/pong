#ifndef BALL_H
#define BALL_H
#include "collider.h"
#include "renderable.h"
#include "stage.h"

#include <cmath>
#include <memory>
struct Speed
{
    int x;
    int y;
    int orig_x;
    int orig_y;

    constexpr Speed(int x, int y) : x{x}, orig_x{x}, y{y}, orig_y{y} {}
    void reverse(CollisionType type)
    {
        switch (type)
        {
        case CollisionType::Horizontal:
        {
            y = -y;
            break;
        }
        case CollisionType::Vertical:
        {
            x = -x;
            break;
        }
        default:
            break;
        }
    }
    void increase(float factor)
    {
        x *= factor;
        y *= factor;
    }
    void reset()
    {
        x = orig_x;
        y = orig_y;
    }
};
static Position operator+(Position pos, Speed speed)
{
    return Position{pos.x + speed.x, pos.y + speed.y};
};
static Position operator+=(Position& pos, Speed speed)
{
    pos = pos + speed;
    return pos;
};
class Ball : public Collider, public Renderable
{
  public:
    constexpr Ball()
        : m_placement{0, 0, 0, 0}, m_prevPlacement{0, 0, 0, 0},
          m_origPlacement{0, 0, 0, 0}, m_speed{0, 0}, m_origSpeed{0, 0} {};

    constexpr Ball(Extent placement, Speed speed)
        : m_placement{placement}, m_prevPlacement{placement},
          m_origPlacement{placement}, m_speed{speed}, m_origSpeed{speed} {};
    void move();
    [[nodiscard]] const Position& getPosition() const noexcept
    {
        return m_placement.pos;
    };
    [[nodiscard]] const Size& getSize() const noexcept
    {
        return m_placement.size;
    };

    [[nodiscard]] const Speed& getSpeed() const noexcept { return m_speed; };

    void collide(CollisionType type) noexcept override
    {
        m_speed.reverse(type);
        if (type == CollisionType::Vertical)
            m_speed.increase(1.1);
    };
    [[nodiscard]] const Extent& getHitbox() const noexcept override
    {
        return m_placement;
    };
    void render(Stage& stage) override;
    void clear(Stage& stage) override;
    void reset()
    {
        m_speed.reset();
        m_placement = m_origPlacement;
    };
    constexpr void setPlacement(Extent placement)
    {
        m_placement = placement;
        m_prevPlacement = placement;
        m_origPlacement = placement;
    };
    constexpr void setSpeed(Speed speed)
    {
        m_speed = speed;
        m_origSpeed = speed;
    }

  private:
    Extent m_placement;
    Extent m_prevPlacement;
    Extent m_origPlacement;
    Speed m_speed;
    Speed m_origSpeed;
};

#endif // BALL_H
