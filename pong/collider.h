#ifndef COLLIDER_H
#define COLLIDER_H

#include "stage.h"

enum class CollisionType { Horizontal, Vertical, Invalid };

template <typename T> class Collider
{
  public:
    constexpr Collider(){};
    virtual ~Collider() = default;
    virtual const Rectangle<T>& getHitbox() const = 0;
    virtual void collide(CollisionType) = 0;
    constexpr static bool overlap(const Rectangle<T>& hitBox1,
                                  const Rectangle<T>& hitBox2)
    {
        if (hitBox1.topLeft().x > hitBox2.topRight().x ||
            hitBox2.topLeft().x > hitBox1.topRight().x)
            return false;
        if (hitBox1.topLeft().y > hitBox2.bottomLeft().y ||
            hitBox2.topLeft().y > hitBox1.bottomLeft().y)
            return false;
        return true;
    };
    constexpr static CollisionType getType(const Rectangle<T>& hitBox1,
                                           const Rectangle<T>& hitBox2)
    {
        if ((hitBox1.topRight().x - hitBox2.topLeft().x == 0 ||
             hitBox2.topRight().x - hitBox1.topLeft().x == 0) &&
            (hitBox1.topRight().x - hitBox2.topRight().x <= hitBox1.width() ||
             hitBox1.topRight().x - hitBox2.topRight().x <= hitBox2.width()))
            return CollisionType::Vertical;
        if ((hitBox1.topLeft().y - hitBox2.bottomLeft().y == 0 ||
             hitBox2.topLeft().y - hitBox1.bottomLeft().y == 0) &&
            (hitBox1.topLeft().y - hitBox2.topLeft().y <= hitBox1.height() ||
             hitBox1.topLeft().y - hitBox2.topLeft().y <= hitBox2.height()))
            return CollisionType::Horizontal;
        return CollisionType::Invalid;
    }
};
#endif // COLLIDER_H
