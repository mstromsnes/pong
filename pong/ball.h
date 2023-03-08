#ifndef BALL_H
#define BALL_H
#include "collider.h"
#include "geometry.h"
#include "renderable.h"
#include "stage.h"

#include <cmath>
#include <concepts>
#include <memory>
template <std::floating_point S>
struct Speed
{
    Vector2D<S> velocity;
    Vector2D<S> origVelocity;

    constexpr Speed(S x, S y) : velocity{x, y}, origVelocity{x, y} {}
    void increase(S increment) { velocity = velocity * 1.02; }
    template <std::floating_point N>
    void reflect(Vector2D<N> normal)
    {
        // auto dotproduct = velocity * normal;
        // auto vec = dotproduct * normal;
        // auto scale = vec * 2.0f;
        // auto result = velocity - scale;
        // velocity = result;
        velocity = velocity - 2.0f * (velocity * normal) * normal;
    }
    void reset() { velocity = origVelocity; }
};
template <typename P, std::floating_point S>
Position<P> operator+(Position<P> pos, Speed<S> speed)
{
    return pos + speed.velocity;
};
template <typename P, std::floating_point S>
Position<P> operator+=(Position<P>& pos, Speed<S> speed)
{
    pos = pos + speed;
    return pos;
};
template <typename T>
class Ball : public Collider<T>, public Renderable
{
  public:
    constexpr Ball() : m_placement{0, 0, 0, 0}, m_origPlacement{0, 0, 0, 0}, m_speed{0, 0}, m_origSpeed{0, 0} {};

    constexpr Ball(Rectangle<T> placement, Speed<double> speed)
        : m_placement{placement}, m_origPlacement{placement}, m_speed{speed}, m_origSpeed{speed} {};
    void move(Stage& stage) { m_placement.translate(m_speed.velocity); }
    void move_custom(Vector2D<double> direction) { m_placement.translate(direction); }
    [[nodiscard]] const Position<T>& getPosition() const noexcept { return m_placement.pos(); };
    [[nodiscard]] const Size<T>& getSize() const noexcept { return m_placement.size(); };

    [[nodiscard]] const Speed<double>& getSpeed() const noexcept { return m_speed; };

    constexpr void collide(Vector2D<double> minimumTranslationVector) noexcept override;
    [[nodiscard]] const Rectangle<T>& getHitbox() const noexcept override { return m_placement; };
    constexpr void reset();
    constexpr void setPlacement(Rectangle<T> placement);
    constexpr void setSpeed(Speed<double> speed);
    constexpr void render(Stage& stage, bool debug = false) override;

  private:
    Rectangle<T> m_placement;
    Rectangle<T> m_origPlacement;
    Speed<double> m_speed;
    Speed<double> m_origSpeed;
    bool m_collision = false;
};

#include "ball.ipp"

#endif // BALL_H
