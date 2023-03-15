#ifndef STAGE_H
#define STAGE_H
#include "drawer.h"
#include "geometry.h"

#include <algorithm>
#include <execution>
class Stage
{
  public:
    constexpr Stage() { clear(); };

    [[nodiscard]] constexpr auto bounds() const -> Size<int> { return Size<int>(WIDTH, HEIGHT); };
    auto getPixmap() const -> GamePixmap const& { return m_pixmap; };
    constexpr void fillRectangle(Rectangle<int> placement, Color color)
    {
        Drawer drawer{m_pixmap};
        auto triangles = placement.getDrawables();
        auto draw_fn = [&](auto triangle) { drawer.drawTriangle(triangle, color); };
        if (std::is_constant_evaluated())
        {
            std::for_each(triangles.begin(), triangles.end(), draw_fn);
        }
        else
        {
            std::for_each(std::execution::unseq, triangles.begin(), triangles.end(), draw_fn);
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
    const int WIDTH = 1280 * 1;
    const int HEIGHT = 700 * 1;
    const int NUM_CHANNELS = 4;
    GamePixmap m_pixmap = GamePixmap(WIDTH, HEIGHT, NUM_CHANNELS);
};

#endif // STAGE_H
