#ifndef RENDERABLE_H
#define RENDERABLE_H
#include "stage.h"

#include <memory>
class Renderable
{
  public:
    constexpr Renderable(){};
    virtual ~Renderable() = default;
    virtual void render(Stage& stage) = 0;
    virtual void clear(Stage& stage) = 0;
};

#endif // RENDERABLE_H
