#ifndef LINE_H
#define LINE_H

#include "constants.h"
#include "vector.h"
template <typename T> class Line
{
  public:
    constexpr Line(Position<T> start_position, Vector2D<double> direction_vec,
                   double len)
        : start_pos{start_position}, end_pos{start_position +
                                             direction_vec * len},
          direction_vector{direction_vec}, len{len}
    {
        clampDirectionVector();
    }
    constexpr Line(Position<T> start_position, Position<T> end_position)
        : start_pos{start_position}, end_pos{end_position},
          direction_vector{start_pos, end_pos}, len{direction_vector.length()}
    {
        direction_vector.normalize();
        clampDirectionVector();
    }
    [[nodiscard]] Vector2D<double> normal() const
    {
        return constants::PiHalfRotationMatrix<double> * direction_vector;
    }
    [[nodiscard]] constexpr Position<T> start() const { return start_pos; };
    [[nodiscard]] constexpr Position<T> end() const { return end_pos; };
    [[nodiscard]] constexpr T highestPoint() const
    {
        return std::min(start_pos.y, end_pos.y);
    };
    [[nodiscard]] constexpr T yFromX(T x)
    {
        double t = static_cast<double>(x - start_pos.x) /
                   static_cast<double>(end_pos.x - start_pos.x);
        return start_pos.y * (1 - t) + end_pos.y * t;
    }
    [[nodiscard]] constexpr T xFromY(T y)
    {
        double t = static_cast<double>(y - start_pos.y) /
                   static_cast<double>(end_pos.y - start_pos.y);
        return start_pos.x * (1 - t) + end_pos.x * t;
    }
    [[nodiscard]] constexpr Position<T> getPos(double t)
    {
        return start_pos * (1 - t) + end_pos * t;
    }

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
