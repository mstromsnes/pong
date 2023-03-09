#ifndef LINE_H
#define LINE_H

#include "constants.h"
#include "vector.h"
template <typename T>
class Line
{
  public:
    constexpr Line(Position<T> start_position, Vector2D<double> direction_vec, double len)
        : start_pos{start_position}, end_pos{start_position + direction_vec * len},
          direction_vector{direction_vec}, len{len}
    {
    }
    constexpr Line(Position<T> start_position, Position<T> end_position)
        : start_pos{start_position}, end_pos{end_position},
          direction_vector{start_pos, end_pos}, len{direction_vector.length()}
    {
        direction_vector.normalize();
    }
    template <std::floating_point V>
    constexpr Line(Vector2D<V> vec)
        : start_pos{constants::Origin<T>}, end_pos{Position<T>{std::lround(vec.x), std::lround(vec.y)}},
          direction_vector{vec}, len{direction_vector.length()}
    {
        direction_vector.normalize();
    }
    [[nodiscard]] Vector2D<double> normal() const { return constants::PiHalfRotationMatrix<double> * direction_vector; }
    [[nodiscard]] constexpr auto start() const { return start_pos; };
    [[nodiscard]] constexpr auto end() const { return end_pos; };
    [[nodiscard]] constexpr auto highestPoint() const { return std::min(start_pos.y, end_pos.y); };
    [[nodiscard]] constexpr auto yFromX(T x) -> T
    {
        double t = static_cast<double>(x - start_pos.x) / static_cast<double>(end_pos.x - start_pos.x);
        return start_pos.y * (1 - t) + end_pos.y * t;
    }
    [[nodiscard]] constexpr auto xFromY(T y) -> T
    {
        double t = static_cast<double>(y - start_pos.y) / static_cast<double>(end_pos.y - start_pos.y);
        return start_pos.x * (1 - t) + end_pos.x * t;
    }
    [[nodiscard]] constexpr auto getPos(double t) -> Position<T> { return start_pos * (1 - t) + end_pos * t; }
    [[nodiscard]] constexpr auto findIntersection(Line<T> otherLine) const -> std::pair<double, double>
    {
        auto x1d = end_pos.x - start_pos.x;                 // -b1
        auto y1d = end_pos.y - start_pos.y;                 // a1
        auto x2d = otherLine.end().x - otherLine.start().x; // -b2
        auto y2d = otherLine.end().y - otherLine.start().y; // a2
        auto xsd = start_pos.x - otherLine.start().x;
        auto ysd = otherLine.start().y - start_pos.y;
        auto divisor = x1d * y2d - y1d * x2d;
        if (divisor == 0)
            return std::make_pair(std::numeric_limits<double>::max(), std::numeric_limits<double>::max());
        double t1 = static_cast<double>(x2d * ysd - y2d * xsd) / static_cast<double>(divisor);
        double t2 = static_cast<double>(x1d * ysd - y1d * xsd) / static_cast<double>(divisor);
        return std::make_pair(t1, t2);
    }
    [[nodiscard]] constexpr auto direction() const { return direction_vector; }
    [[nodiscard]] constexpr auto length() const { return len; }

  private:
    constexpr void clampDirectionVector()
    {
        if (direction_vector.y < 0)
        {
            std::swap(start_pos, end_pos);
            direction_vector = -direction_vector;
        }
    }
    Position<T> start_pos;
    Position<T> end_pos;
    Vector2D<double> direction_vector;
    double len;
};

#endif // LINE_H
