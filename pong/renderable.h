#ifndef RENDERABLE_H
#define RENDERABLE_H
#include "stage.h"

#include <memory>
class Renderable
{
  public:
    constexpr Renderable(Color baseColor = Color(0x00, 0x00, 0xff),
                         Color debugColor = Color(0xff, 0x00, 0x00))
        : m_baseColor{baseColor}, m_debugColor{debugColor} {};
    virtual ~Renderable() = default;
    virtual void render(Stage& stage, bool debug = false) = 0;
    [[nodiscard]] auto getColor() const -> Color const& { return m_baseColor; }
    [[nodiscard]] auto getDebugColor() const -> Color const&
    {
        return m_debugColor;
    }

  private:
    Color m_baseColor;
    Color m_debugColor;
};

#endif // RENDERABLE_H
