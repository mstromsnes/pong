#ifndef DRAWER_H
#define DRAWER_H

#include "geometry/triangle.h"

#include <cmath>
#include <vector>

struct TriangleDrawParams
{
    float xstart;
    float xend;
    int ystart;
    int yend;
    float dxleft;
    float dxright;
};
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
class GamePixmap
{
  public:
    constexpr GamePixmap(int width, int height, int channels)
        : num_channels{channels}, m_pixmap(width * height * num_channels, 0x00),
          m_width{width}, m_height{height} {};
    constexpr int getChannelCount() const { return num_channels; };
    constexpr int getWidth() const { return m_width; };
    constexpr int getHeight() const { return m_height; };
    constexpr int getRowSize() const { return m_width * num_channels; }
    constexpr uint8_t& operator[](int idx) { return m_pixmap[idx]; }
    constexpr auto* data() const { return m_pixmap.data(); }

  private:
    int num_channels;
    std::vector<uint8_t> m_pixmap;
    int m_width;
    int m_height;
};
class Drawer
{
  public:
    constexpr Drawer(GamePixmap& pixmap) : m_pixmap{pixmap} {}
    constexpr void drawHorizontalLine(int xs, int xe, int y, Color color)
    {
        for (int x = xs; x < xe; x++)
        {
            paintPixel(x, y, color);
        }
    };
    constexpr void drawTriangle(Triangle<int> triangle, Color color)
    {
        auto params = findInitialConditions(triangle);
        if (params.ystart >= 0 || params.yend < m_pixmap.getHeight())
        {
            params.ystart = std::clamp(params.ystart, 0, m_pixmap.getHeight());
            params.yend = std::clamp(params.yend, 0, m_pixmap.getHeight());
            for (int y = params.ystart; y < params.yend; y++)
            {
                int xstart_r = std::lround(params.xstart);
                int xend_r = std::lround(params.xend);
                if (xstart_r >= 0 || xend_r < m_pixmap.getWidth())
                {
                    drawHorizontalLine(xstart_r, xend_r, y, color);
                    params.xstart += params.dxleft;
                    params.xend += params.dxright;
                }
            }
        }
    }
    template <typename T> constexpr void drawLine(Line<T> line, Color color)
    {
        // Bresenham line drawing algorithm that reverts to naive for axis
        // aligned lines.
        auto [x0, x1, y0, y1] = findEndPoints(line);
        if (x1 < x0)
        {
            // We want to draw from left to right
            std::swap(x1, x0);
            std::swap(y1, y0);
        }
        if (y1 == y0)
        {
            drawHorizontalLine(x0, x1, y0, color);
            return;
        }
        if (x0 == x1)
        {
            // Here we iterate y, so we want to draw from top to bottom
            if (y1 < y0)
                std::swap(y1, y0);
            drawVerticalLine(x0, y0, y1, color);
            return;
        }
        auto c = static_cast<float>(y1 - y0) / static_cast<float>(x1 - x0);
        for (int x = x0; x < x1; x++)
        {
            auto y = std::lround(c * (x - x0) + y0);
            paintPixel(x, y, color);
        }
    }
    constexpr void drawVerticalLine(int x, int y0, int y1, Color color)
    {
        for (int y = y0; y < y1; y++)
        {
            paintPixel(x, y, color);
        }
    }
    constexpr void paintPixel(int x, int y, Color color)
    {
        if (x < 0 || x >= m_pixmap.getWidth() || y < 0 ||
            y >= m_pixmap.getHeight())
            return;
        for (int j = 0; j < m_pixmap.getChannelCount(); j++)
        {
            auto idx =
                y * m_pixmap.getRowSize() + x * m_pixmap.getChannelCount() + j;
            m_pixmap[idx] = color[j];
        }
    }
    template <typename T>
    constexpr void drawLineNormal(Line<T> line, Color color)
    // Draws a line normal to the line given, starting at the middle of the line
    // given.
    {
        auto normalVec = line.normal();
        auto length = std::min<double>(line.length(), 10.0f);
        auto normalLine =
            Line<T>(line.start() + line.direction() * line.length() / 2.0f,
                    normalVec, length);
        drawLine(normalLine, color);
    }

  private:
    [[nodiscard]] constexpr static TriangleDrawParams
    findInitialConditions(const Triangle<int>& tri)
    {
        auto vertices{tri.getVertices()};
        float xstart = vertices[0].x;
        int ystart = vertices[0].y;
        if (vertices[0].y == vertices[1].y)
        {
            float dxleft = static_cast<float>(vertices[2].x - vertices[0].x) /
                           static_cast<float>(vertices[2].y - vertices[0].y);
            float dxright = static_cast<float>(vertices[2].x - vertices[1].x) /
                            static_cast<float>(vertices[2].y - vertices[1].y);
            float xend = vertices[1].x;
            int yend = vertices[2].y;

            return TriangleDrawParams(xstart, xend, ystart, yend, dxleft,
                                      dxright);
        }
        else
        {
            float dxleft = static_cast<float>(vertices[1].x - vertices[0].x) /
                           static_cast<float>(vertices[1].y - vertices[0].y);
            float dxright = static_cast<float>(vertices[2].x - vertices[0].x) /
                            static_cast<float>(vertices[2].y - vertices[0].y);
            float xend = vertices[0].x;
            int yend = vertices[1].y;
            return TriangleDrawParams(xstart, xend, ystart, yend, dxleft,
                                      dxright);
        }
    };
    template <typename T>
    constexpr static auto findEndPoints(Line<T> line)
        -> std::tuple<int, int, int, int>
    {
        if constexpr (std::is_integral_v<T>)
        {
            auto x0 = line.start().x;
            auto x1 = line.end().x;
            auto y0 = line.start().y;
            auto y1 = line.end().y;
            return std::tuple{x0, x1, y0, y1};
        }
        else
        {
            auto x0 = std::lround(line.start().x);
            auto x1 = std::lround(line.end().x);
            auto y0 = std::lround(line.start().y);
            auto y1 = std::lround(line.end().y);
            return std::tuple{x0, x1, y0, y1};
        }
    }

    GamePixmap& m_pixmap;
};

#endif // DRAWER_H
