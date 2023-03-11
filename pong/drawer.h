#ifndef DRAWER_H
#define DRAWER_H

#include "geometry/triangle.h"

#include <cmath>
#include <ranges>
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
    constexpr Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r{r}, g{g}, b{b}, a{a} {};
    constexpr Color(uint8_t r, uint8_t g, uint8_t b) : r{r}, g{g}, b{b}, a{0xff} {};
    uint8_t r, g, b, a;

    constexpr Color(uint8_t grey) : r{grey}, g{grey}, b{grey}, a{0xff} {};

    constexpr void setRed(uint8_t r) { r = r; };
    constexpr void setGreen(uint8_t g) { g = g; };
    constexpr void setBlue(uint8_t b) { b = b; };
    constexpr void setAlpha(uint8_t a) { a = a; };

    constexpr auto operator[](int index) -> uint8_t&
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
        : num_channels{channels}, m_pixmap(width * height * num_channels, 0x00), m_width{width}, m_height{height},
          m_clear_pixmap{clearStage(width, height, channels)} {};
    constexpr auto getChannelCount() const { return num_channels; };
    constexpr auto getWidth() const { return m_width; };
    constexpr auto getHeight() const { return m_height; };
    constexpr auto getRowSize() const { return m_width * num_channels; }
    auto operator[](int idx) -> uint8_t& { return m_pixmap[idx]; }
    auto* data() const { return m_pixmap.data(); }
    constexpr void clear() { m_pixmap = m_clear_pixmap; }
    constexpr auto clearStage(int width, int height, int channels) -> std::vector<uint8_t>
    {
        auto stage = std::vector<uint8_t>(width * height * num_channels, 0x00);
        auto view = std::ranges::subrange(stage.begin() + 3, stage.end()) | std::ranges::views::stride(4);
        for (auto& alpha : view)
        {
            alpha = 0xff;
        }
        return stage;
    }

  private:
    int num_channels;
    std::vector<uint8_t> m_pixmap;
    const std::vector<uint8_t> m_clear_pixmap;
    int m_width;
    int m_height;
};
class Drawer
{
  public:
    constexpr Drawer(GamePixmap& pixmap) : m_pixmap{pixmap} {}
    constexpr void drawHorizontalLine(int xs, int xe, int y, Color color);
    constexpr void drawTriangle(Triangle<int> triangle, Color color);
    constexpr void drawVerticalLine(int x, int y0, int y1, Color color);
    constexpr void paintPixel(int x, int y, Color color);
    constexpr void clear() { m_pixmap.clear(); };

    template <typename T>
    constexpr void drawLineNormal(Line<T> line, Color color);
    template <typename T>
    constexpr void drawLine(Line<T> line, Color color);

  private:
    [[nodiscard]] constexpr static auto findInitialConditions(const Triangle<int>& tri) -> TriangleDrawParams;
    template <typename T>
    constexpr static auto findEndPoints(Line<T> line) -> std::tuple<int, int, int, int>;

    GamePixmap& m_pixmap;
};
#include "drawer.ipp"

#endif // DRAWER_H
