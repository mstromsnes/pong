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
    void increase(S increment) { velocity = velocity * 1.02; }
    template <std::floating_point N> void reflect(Vector2D<N> normal)
    {
        auto dotproduct = velocity * normal;
        auto vec = dotproduct * normal;
        auto scale = vec * 2.0f;
        auto result = velocity - scale;
        velocity = result;
    }
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
        : m_placement{0, 0, 0, 0}, m_origPlacement{0, 0, 0, 0}, m_speed{0, 0},
          m_origSpeed{0, 0} {};

    constexpr Ball(Rectangle<T> placement, Speed<double> speed)
        : m_placement{placement}, m_origPlacement{placement}, m_speed{speed},
          m_origSpeed{speed} {};
    void move(Stage& stage) { m_placement.translate(m_speed.velocity); }
    void move_custom(Vector2D<double> direction)
    {
        m_placement.translate(direction);
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

    void collide(Vector2D<double> minimumTranslationVector) noexcept override
    {
        m_placement.translate(minimumTranslationVector);
        m_speed.reflect(minimumTranslationVector.normalized());
        m_speed.increase(0.2);
        collision = true;
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
        m_origPlacement = placement;
    };
    constexpr void setSpeed(Speed<double> speed)
    {
        m_speed = speed;
        m_origSpeed = speed;
    }
    void render(Stage& stage) override
    {
        auto color =
            collision ? Color(0xff, 0x00, 0x00) : Color(0x00, 0x00, 0xff);
        stage.fillRectangle(m_placement, color);
        collision = false;
    }

  private:
    Rectangle<T> m_placement;
    Rectangle<T> m_origPlacement;
    Speed<double> m_speed;
    Speed<double> m_origSpeed;
    bool collision = false;
};

#endif // BALL_H
