#ifndef GEOMETRY_H
#define GEOMETRY_H

struct Position
{
    int x;
    int y;
};

struct Size
{
    int width;
    int height;
};

struct Extent
{
    constexpr Extent(int x, int y, int w, int h) : pos{x, y}, size{w, h} {};
    constexpr Extent(Position p, int w, int h) : pos{p}, size{w, h} {};
    constexpr Extent(int x, int y, Size s) : pos{x, y}, size{s} {};
    Position pos;
    Size size;
    [[nodiscard]] constexpr int left() const { return pos.x; };
    [[nodiscard]] constexpr int right() const { return pos.x + size.width; };
    [[nodiscard]] constexpr int top() const { return pos.y; };
    [[nodiscard]] constexpr int bottom() const { return pos.y + size.height; };
    [[nodiscard]] constexpr Position topLeft() const { return pos; };
    [[nodiscard]] constexpr Position topRight() const
    {
        return Position{right(), top()};
    };
    [[nodiscard]] constexpr Position bottomLeft() const
    {
        return Position{left(), bottom()};
    };
    [[nodiscard]] constexpr Position bottomRight() const
    {
        return Position{right(), bottom()};
    };
    [[nodiscard]] constexpr Position center() const
    {
        return Position{(left() + right()) / 2, (top() + bottom()) / 2};
    };
    [[nodiscard]] constexpr bool contains(int x, int y)
    {
        return !(y < top() || y > bottom() || x < left() || x > right());
    };
};

#endif // GEOMETRY_H
