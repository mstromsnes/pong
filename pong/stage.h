#ifndef STAGE_H
#define STAGE_H
#include "drawer.h"
#include "geometry.h"

class Stage
{
  public:
    constexpr Stage() { clear(); };

    [[nodiscard]] constexpr auto bounds() const -> Size<int> { return Size<int>(WIDTH, HEIGHT); };
    auto getPixmap() const -> GamePixmap const& { return m_pixmap; };
    constexpr void fillRectangle(Rectangle<int> placement, Color color)
    {
        Drawer drawer{m_pixmap};
        for (auto const& triangle : placement.getDrawables())
        {
            drawer.drawTriangle(triangle, color);
        }
    };
    template <typename T>
    constexpr void drawNormal(Line<T> line, Color color)
    {
        Drawer drawer{m_pixmap};
        drawer.drawLineNormal(line, color);
    }
    constexpr void clear()
    {
        Drawer drawer{m_pixmap};
        drawer.clear();
    }

  private:
    const int WIDTH = 640;
    const int HEIGHT = 360;
    const int NUM_CHANNELS = 4;
    GamePixmap m_pixmap = GamePixmap(WIDTH, HEIGHT, NUM_CHANNELS);
};

#endif // STAGE_H
