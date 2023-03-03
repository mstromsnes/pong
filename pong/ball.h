#ifndef BALL_H
#define BALL_H
#include "collider.h"
#include "geometry.h"
#include "renderable.h"
#include "stage.h"

#include <cmath>
#include <concepts>
#include <memory>
template <std::floating_point S> struct Speed
{
    Vector2D<S> velocity;
    Vector2D<S> origVelocity;

    constexpr Speed(S x, S y) : velocity{x, y}, origVelocity{x, y} {}
    void reverse(CollisionType type)
    {
        switch (type)
        {
        case CollisionType::Horizontal:
        {
            velocity.y = -velocity.y;
            break;
        }
        case CollisionType::Vertical:
        {
            velocity.x = -velocity.x;
            break;
        }
        default:
            break;
        }
    }
    void increase(S increment) { velocity = velocity + increment; }
    void reset() { velocity = origVelocity; }
};
template <typename P, std::floating_point S>
static Position<P> operator+(Position<P> pos, Speed<S> speed)
{
    return pos + speed.velocity;
};
template <typename P, std::floating_point S>
static Position<P> operator+=(Position<P>& pos, Speed<S> speed)
{
    pos = pos + speed;
    return pos;
};
template <typename T> class Ball : public Collider<T>, public Renderable
{
  public:
    constexpr Ball()
        : m_placement{0, 0, 0, 0}, m_prevPlacement{0, 0, 0, 0},
          m_origPlacement{0, 0, 0, 0}, m_speed{0, 0}, m_origSpeed{0, 0} {};

    constexpr Ball(Rectangle<T> placement, Speed<float> speed)
        : m_placement{placement}, m_prevPlacement{placement},
          m_origPlacement{placement}, m_speed{speed}, m_origSpeed{speed} {};
    void move()
    {
        m_prevPlacement = m_placement;
        m_placement.translate(m_speed.velocity);
    }
    [[nodiscard]] const Position<T>& getPosition() const noexcept
    {
        return m_placement.pos();
    };
    [[nodiscard]] const Size<T>& getSize() const noexcept
    {
        return m_placement.size();
    };

    [[nodiscard]] const Speed<float>& getSpeed() const noexcept
    {
        return m_speed;
    };

    void collide(CollisionType type) noexcept override
    {
        m_speed.reverse(type);
        if (type == CollisionType::Vertical)
            m_speed.increase(0.2);
    };
    [[nodiscard]] const Rectangle<T>& getHitbox() const noexcept override
    {
        return m_placement;
    };
    void reset()
    {
        m_speed.reset();
        m_placement = m_origPlacement;
    };
    constexpr void setPlacement(Rectangle<T> placement)
    {
        m_placement = placement;
        m_prevPlacement = placement;
        m_origPlacement = placement;
    };
    constexpr void setSpeed(Speed<float> speed)
    {
        m_speed = speed;
        m_origSpeed = speed;
    }
    void render(Stage& stage) override
    {
        clear(stage);
        stage.fill(m_placement, Color(0x00, 0x00, 0xff));
    }
    void clear(Stage& stage) override { stage.fill(m_prevPlacement, 0x0); }

  private:
    Rectangle<T> m_placement;
    Rectangle<T> m_prevPlacement;
    Rectangle<T> m_origPlacement;
    Speed<float> m_speed;
    Speed<float> m_origSpeed;
};

#endif // BALL_H
