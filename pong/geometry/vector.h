#ifndef VECTOR_H
#define VECTOR_H
#include "matrix.h"

#include <cmath>
#include <concepts>
template <typename T>
struct Position;

template <std::floating_point V>
struct Vector2D
{
    V x;
    V y;
    template <typename P>
    constexpr Vector2D<V>(Position<P> a, Position<P> b) : x{static_cast<V>(b.x - a.x)}, y{static_cast<V>(b.y - a.y)}
    {
    }
    template <typename P>
    constexpr Vector2D<V>(Position<P> pos) : x{static_cast<V>(pos.x)}, y{static_cast<V>(pos.y)}
    {
    }
    constexpr Vector2D<V>(V x, V y) : x{x}, y{y} {}
    [[nodiscard]] Vector2D<V> operator-() const { return Vector2D<V>(-x, -y); };
    [[nodiscard]] constexpr V length() const { return std::hypot(x, y); }
    constexpr void normalize()
    {
        auto len = length();
        x = x / len;
        y = y / len;
    }
    constexpr Vector2D<V> normalized() const { return Vector2D<V>(x / length(), y / length()); }
    constexpr Vector2D<V> operator+(V right) const { return Vector2D<V>(x + right, y + right); }
    template <typename T>
    constexpr Vector2D<V> operator/(T divisor)
    {
        return Vector2D<V>{x / divisor, y / divisor};
    }
};

template <typename P>
struct Position
{
    P x;
    P y;
    constexpr Position<P> operator-() const { return Position<P>(-x, -y); };
    void operator+=(const Position& right)
    {
        x += right.x;
        y += right.y;
    };
    template <std::floating_point V>
    void operator+=(const Vector2D<V>& right)
    {
        if constexpr (std::is_integral_v<P>)
        {
            x += std::lround(right.x);
            y += std::lround(right.y);
        }
        else
        {
            x += right.x;
            y += right.y;
        }
    };
};
template <typename T>
concept arithmetic = std::integral<T> || std::floating_point<T>;

template <typename M, std::floating_point V>
[[nodiscard]] constexpr Vector2D<V> operator*(const Matrix3X3<M>& mat, const Vector2D<V>& vec)
{
    return Vector2D<V>(vec.x * mat.xx + vec.y * mat.xy + 1 * mat.xz, vec.x * mat.yx + vec.y * mat.yy + 1 * mat.yz);
}
template <std::floating_point V, arithmetic S>
[[nodiscard]] constexpr Vector2D<V> operator*(const Vector2D<V>& vec, const S& scalar)
{
    return Vector2D<V>(vec.x * scalar, vec.y * scalar);
};
template <std::floating_point V, arithmetic S>
[[nodiscard]] constexpr Vector2D<V> operator*(const S& scalar, const Vector2D<V>& vec)
{
    return Vector2D<V>(vec.x * scalar, vec.y * scalar);
};
template <std::floating_point V1, std::floating_point V2>
[[nodiscard]] constexpr auto operator-(const Vector2D<V1>& left, const Vector2D<V2>& right)
    -> Vector2D<decltype(left.x - right.x)>
{
    return Vector2D(left.x - right.x, left.y - right.y);
}
template <typename P>
[[nodiscard]] constexpr Position<P> operator+(const Position<P>& left, const Position<P>& right)
{
    return Position<P>(left.x + right.x, left.y + right.y);
}
template <std::signed_integral P, std::floating_point V>
[[nodiscard]] constexpr Position<P> operator+(const Position<P>& pos, const Vector2D<V>& vec)
{
    return Position<P>(pos.x + std::lround(vec.x), pos.y + std::lround(vec.y));
};
template <std::floating_point P, std::floating_point V>
[[nodiscard]] constexpr Position<P> operator+(const Position<P>& pos, const Vector2D<V>& vec)
{
    return Position<P>(pos.x + vec.x, pos.y + vec.y);
};
template <std::signed_integral P>
[[nodiscard]] constexpr Vector2D<double> operator-(const Position<P>& pos1, const Position<P>& pos2)
{
    return Vector2D(static_cast<double>(pos2.x - pos1.x), static_cast<double>(pos2.y - pos1.y));
}
template <std::floating_point P>
[[nodiscard]] constexpr Vector2D<P> operator-(const Position<P>& pos1, const Position<P>& pos2)
{
    return Vector2D(pos2.x - pos1.x, pos2.y - pos1.y);
}
template <std::floating_point V1, std::floating_point V2>
[[nodiscard]] constexpr auto operator*(const Vector2D<V1>& left, const Vector2D<V2>& right)
    -> decltype(left.x * right.x)
{
    return left.x * right.x + left.y * right.y;
};
template <typename M, std::floating_point P>
[[nodiscard]] constexpr Position<P> operator*(const Matrix3X3<M>& left, Position<P> right)
{
    return Position(left.xx * right.x + left.xy * right.y + left.xz * 1,
                    left.yx * right.x + left.yy * right.y + left.yz * 1);
};
template <typename M, std::signed_integral P>
[[nodiscard]] constexpr Position<P> operator*(const Matrix3X3<M>& left, Position<P> right)
{
    return Position<P>(std::lround(left.xx * right.x + left.xy * right.y + left.xz * 1),
                       std::lround(left.yx * right.x + left.yy * right.y + left.yz * 1));
};
template <std::floating_point V>
[[nodiscard]] constexpr auto operator==(Vector2D<V> const& left, Vector2D<V> const& right) -> bool
{
    return left.x == right.x && left.y == right.y;
}
#endif // VECTOR_H
