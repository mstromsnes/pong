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
        if (hitBox1.left() > hitBox2.right() ||
            hitBox2.left() > hitBox1.right())
            return false;
        if (hitBox1.top() > hitBox2.bottom() ||
            hitBox2.top() > hitBox1.bottom())
            return false;
        return true;
    };
};
#endif // COLLIDER_H
