#ifndef STAGE_H
#define STAGE_H

#include "geometry.h"

#include <algorithm>
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

    [[nodiscard]] Position stageToVpCoords(Position stagePos) const;
    [[nodiscard]] Position vpToStageCoords(Position vpCoords) const;
    [[nodiscard]] constexpr auto& bounds() const { return m_stageBounds; };
    const GamePixmap& getPixmap() const { return m_pixmap; };
    constexpr void fill(Extent placement, Color color)
    {
        // int lowerBound =
        //     (placement.top() * m_stageBounds.size.width + placement.left()) *
        //     NUM_CHANNELS;
        // lowerBound = std::clamp<int>(lowerBound, 0, m_pixmap.size());
        // int upperBound = ((placement.bottom() - 1) * m_stageBounds.size.width
        // +
        //                   placement.right()) *
        //                  NUM_CHANNELS;
        // upperBound = std::clamp<int>(upperBound, 0, m_pixmap.size());
        if (!areaInBounds(placement))
            return;
        int pixmap_width = m_stageBounds.size.width * NUM_CHANNELS;
        int topmost_y = placement.top();
        int bottommost_y = placement.bottom();
        int leftmost_x = placement.left() * NUM_CHANNELS;
        int rightmost_x = placement.right() * NUM_CHANNELS;
        leftmost_x = std::clamp<int>(leftmost_x, 0, pixmap_width);
        rightmost_x = std::clamp<int>(rightmost_x, 0, pixmap_width);
        bottommost_y =
            std::clamp<int>(bottommost_y, 0, m_stageBounds.size.height);
        topmost_y = std::clamp<int>(topmost_y, 0, m_stageBounds.size.height);

        for (int i = topmost_y; i < bottommost_y; i++)
        {
            for (int j = leftmost_x; j < rightmost_x; j += NUM_CHANNELS)
            {
                // int x = (i / NUM_CHANNELS) % m_stageBounds.size.width;
                // int y = (i / NUM_CHANNELS) / m_stageBounds.size.width;
                // if (placement.contains(x, y))
                for (int k = 0; k < NUM_CHANNELS; k++)
                {
                    int idx = i * pixmap_width + j + k;
                    if (idx < m_pixmap.size())
                        m_pixmap[i * pixmap_width + j + k] = color[k];
                }
            }
        }
    };

  private:
    bool stageCoordsInRange(Position stagePos) const;
    const Extent m_stageBounds{0, 0, 320, 240};

    static const uint8_t NUM_CHANNELS = 4;
    GamePixmap m_pixmap = GamePixmap(
        m_stageBounds.size.width * m_stageBounds.size.height * NUM_CHANNELS,
        0x00);
    constexpr bool areaInBounds(Extent placement)
    {
        int pixmap_width = m_stageBounds.size.width * NUM_CHANNELS;
        int leftmost_x = placement.left() * NUM_CHANNELS;
        int rightmost_x = placement.right() * NUM_CHANNELS;
        if (leftmost_x > pixmap_width || rightmost_x < 0)
            return false;

        int pixmap_height = m_stageBounds.size.height;
        int topmost_y = placement.top();
        int bottommost_y = placement.bottom();
        if (bottommost_y < 0 || topmost_y > pixmap_height)
            return false;
        return true;
    };
};

#endif // STAGE_H
