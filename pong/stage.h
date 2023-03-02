#ifndef STAGE_H
#define STAGE_H

#include "geometry.h"

#include <algorithm>
#include <cmath>
#include <vector>
struct Color
{
    constexpr Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
        : r{r}, g{g}, b{b}, a{a} {};
    constexpr Color(uint8_t r, uint8_t g, uint8_t b)
        : r{r}, g{g}, b{b}, a{0xff} {};
    constexpr Color(uint8_t grey) : r{grey}, g{grey}, b{grey}, a{0xff} {};
    constexpr void setRed(uint8_t r) { r = r; };
    constexpr void setGreen(uint8_t g) { g = g; };
    constexpr void setBlue(uint8_t b) { b = b; };
    constexpr void setAlpha(uint8_t a) { a = a; };
    uint8_t r, g, b, a;
    constexpr uint8_t& operator[](int index)
    {
        switch (index)
        {
        case 0:
            return r;
        case 1:
            return g;
        case 2:
            return b;
        case 3:
        default:
            return a;
        }
    }
};
using GamePixmap = std::vector<uint8_t>;
class Stage
{
  public:
    constexpr Stage() { fill(m_stageBounds, 0x00); };

    [[nodiscard]] Position<int> stageToVpCoords(Position<int> stagePos) const;
    [[nodiscard]] Position<int> vpToStageCoords(Position<int> vpCoords) const;
    [[nodiscard]] constexpr auto& bounds() const { return m_stageBounds; };
    const GamePixmap& getPixmap() const { return m_pixmap; };
    constexpr void drawHorizontalLine(int xs, int xe, int y, Color color)
    {
        int offset = y * m_stageBounds.width() * NUM_CHANNELS;
        for (int i = xs; i < xe; i++)
        {
            for (int j = 0; j < NUM_CHANNELS; j++)
            {
                m_pixmap[offset + i * NUM_CHANNELS + j] = color[j];
            }
        }
    };
    constexpr void drawTriangle(Triangle<int> triangle, Color color)
    {
        auto vertices = triangle.getVertices();
        auto findInitialConditions =
            [&vertices]() -> std::tuple<float, float, int, int, float, float> {
            float xstart = vertices[0].x;
            int ystart = vertices[0].y;
            if (vertices[0].y == vertices[1].y)
            {
                float dxleft =
                    static_cast<float>(vertices[2].x - vertices[0].x) /
                    static_cast<float>(vertices[2].y - vertices[0].y);
                float dxright =
                    static_cast<float>(vertices[2].x - vertices[1].x) /
                    static_cast<float>(vertices[2].y - vertices[1].y);
                float xend = vertices[1].x;
                int yend = vertices[2].y;

                return std::make_tuple(xstart, xend, ystart, yend, dxleft,
                                       dxright);
            }
            else
            {
                float dxleft =
                    static_cast<float>(vertices[1].x - vertices[0].x) /
                    static_cast<float>(vertices[1].y - vertices[0].y);
                float dxright =
                    static_cast<float>(vertices[2].x - vertices[0].x) /
                    static_cast<float>(vertices[2].y - vertices[0].y);
                float xend = vertices[0].x;
                int yend = vertices[1].y;
                return std::make_tuple(xstart, xend, ystart, yend, dxleft,
                                       dxright);
            }
        };
        auto [xstart, xend, ystart, yend, dxleft, dxright] =
            findInitialConditions();
        if (ystart >= m_stageBounds.topLeft().y ||
            yend < m_stageBounds.height())
        {
            ystart = std::clamp(ystart, m_stageBounds.topLeft().y,
                                m_stageBounds.height());
            yend = std::clamp(yend, m_stageBounds.topLeft().y,
                              m_stageBounds.height());
            for (int y = ystart; y < yend; y++)
            {
                int xstart_r = std::lround(xstart);
                int xend_r = std::lround(xend);
                if (xstart_r >= m_stageBounds.topLeft().x ||
                    xend_r < m_stageBounds.width())
                {
                    xstart_r = std::clamp(xstart_r, m_stageBounds.topLeft().x,
                                          m_stageBounds.width());
                    xend_r = std::clamp(xend_r, m_stageBounds.topLeft().x,
                                        m_stageBounds.width());
                    drawHorizontalLine(xstart_r, xend_r, y, color);
                    xstart += dxleft;
                    xend += dxright;
                }
            }
        }
    }
    constexpr void fill(Rectangle<int> placement, Color color)
    {
        for (const auto& triangle : placement.getDrawables())
        {
            drawTriangle(triangle, color);
        }
    };

  private:
    bool stageCoordsInRange(Position<int> stagePos) const;
    const Rectangle<int> m_stageBounds{0, 0, 320, 200};

    static const uint8_t NUM_CHANNELS = 4;
    GamePixmap m_pixmap = GamePixmap(
        m_stageBounds.width() * m_stageBounds.height() * NUM_CHANNELS, 0x00);
    constexpr bool areaInBounds(Rectangle<int> placement)
    {
        int pixmap_width = m_stageBounds.width() * NUM_CHANNELS;
        int leftmost_x = placement.topLeft().x * NUM_CHANNELS;
        int rightmost_x = placement.topRight().x * NUM_CHANNELS;
        if (leftmost_x > pixmap_width || rightmost_x < 0)
            return false;

        int pixmap_height = m_stageBounds.height();
        int topmost_y = placement.topLeft().y;
        int bottommost_y = placement.bottomLeft().y;
        if (bottommost_y < 0 || topmost_y > pixmap_height)
            return false;
        return true;
    };
};

#endif // STAGE_H
