#ifndef COLLIDER_H
#define COLLIDER_H

#include "stage.h"

enum class CollisionType { Horizontal, Vertical, Invalid };

class Collider
{
  public:
    constexpr Collider(){};
    virtual ~Collider() = default;
    virtual const Extent& getHitbox() const = 0;
    virtual void collide(CollisionType) = 0;
    constexpr static bool overlap(const Extent& hitBox1, const Extent& hitBox2)
    {
        if (hitBox1.left() > hitBox2.right() ||
            hitBox2.left() > hitBox1.right())
            return false;
        if (hitBox1.top() > hitBox2.bottom() ||
            hitBox2.top() > hitBox1.bottom())
            return false;
        return true;
    };
    constexpr static CollisionType getType(const Extent& hitBox1,
                                           const Extent& hitBox2)
    {
        if ((hitBox1.right() - hitBox2.left() == 0 ||
             hitBox2.right() - hitBox1.left() == 0) &&
            (hitBox1.right() - hitBox2.right() <= hitBox1.size.width ||
             hitBox1.right() - hitBox2.right() <= hitBox2.size.width))
            return CollisionType::Vertical;
        if ((hitBox1.top() - hitBox2.bottom() == 0 ||
             hitBox2.top() - hitBox1.bottom() == 0) &&
            (hitBox1.top() - hitBox2.top() <= hitBox1.size.height ||
             hitBox1.top() - hitBox2.top() <= hitBox2.size.height))
            return CollisionType::Horizontal;
        return CollisionType::Invalid;
    }
};
#endif // COLLIDER_H
