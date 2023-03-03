#ifndef STAGE_H
#define STAGE_H

#include "drawer.h"
#include "geometry.h"

class Stage
{
  public:
    constexpr Stage() { fill(Rectangle<int>(0, 0, WIDTH, HEIGHT), 0x00); };

    [[nodiscard]] constexpr auto bounds() const
    {
        return Size<int>(WIDTH, HEIGHT);
    };
    const GamePixmap& getPixmap() const { return m_pixmap; };
    constexpr void fill(Rectangle<int> placement, Color color)
    {
        Drawer drawer{m_pixmap};
        for (const auto& triangle : placement.getDrawables())
        {
            drawer.drawTriangle(triangle, color);
        }
    };

  private:
    const int WIDTH = 320;
    const int HEIGHT = 200;
    const int NUM_CHANNELS = 4;
    GamePixmap m_pixmap = GamePixmap(WIDTH, HEIGHT, NUM_CHANNELS);
};

#endif // STAGE_H
