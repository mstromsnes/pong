#ifndef RECTANGLE_H
#define RECTANGLE_H
#include "constants.h"
#include "line.h"
#include "matrix.h"
#include "polygon.h"
#include "quadrilateral.h"
#include "triangle.h"
#include "vector.h"

#include <algorithm>
#include <array>
#include <functional>
#include <numbers>
template <typename T>
struct Size
{
    T width;
    T height;
};
template <typename T>
class Rectangle : public ConvexPolygon<T, 4>
{
  public:
    constexpr Rectangle(T x, T y, T w, T h)
        : unrotatedPos{x, y}, m_size{w, h}, orientation{0},
          m_rotationAroundOriginMatrix{calculateRotationAroundOriginMatrix(orientation)},
          m_translationToOriginMatrix{calculateTranslationToOriginMatrix(center())},
          m_rotationAroundCenterMatrix{
              calculateRotationAroundCenterMatrix(m_rotationAroundOriginMatrix, m_translationToOriginMatrix)}
    {
    }
    constexpr Rectangle(Position<T> p, T w, T h)
        : unrotatedPos{p}, m_size{w, h}, orientation{0},
          m_rotationAroundOriginMatrix{calculateRotationAroundOriginMatrix(orientation)},
          m_translationToOriginMatrix{calculateTranslationToOriginMatrix(center())},
          m_rotationAroundCenterMatrix{
              calculateRotationAroundCenterMatrix(m_rotationAroundOriginMatrix, m_translationToOriginMatrix)}
    {
    }
    constexpr Rectangle(T x, T y, Size<T> s)
        : unrotatedPos{x, y}, m_size{s}, orientation{0},
          m_rotationAroundOriginMatrix{calculateRotationAroundOriginMatrix(orientation)},
          m_translationToOriginMatrix{calculateTranslationToOriginMatrix(center())},
          m_rotationAroundCenterMatrix{
              calculateRotationAroundCenterMatrix(m_rotationAroundOriginMatrix, m_translationToOriginMatrix)}
    {
    }
    [[nodiscard]] virtual auto lines() const -> std::array<Line<T>, 4> override
    {
        auto lines =
            std::array<Line<T>, 4>{Line(topLeft(), m_rotationAroundOriginMatrix * constants::XUnitVector<double>,
                                        static_cast<double>(m_size.width)),
                                   Line(topRight(), m_rotationAroundOriginMatrix * constants::YUnitVector<double>,
                                        static_cast<double>(m_size.height)),
                                   Line(bottomRight(), m_rotationAroundOriginMatrix * constants::XUnitVector<double>,
                                        static_cast<double>(m_size.width)),
                                   Line(bottomLeft(), m_rotationAroundOriginMatrix * constants::YUnitVector<double>,
                                        static_cast<double>(m_size.height))};
        std::ranges::sort(lines, [](Line<T> a, Line<T> b) { return a.highestPoint() > b.highestPoint(); });
        return lines;
    };
    [[nodiscard]] virtual auto normals() const -> std::array<Vector2D<double>, 4> override
    {
        auto v{verticesCCW()};
        return std::array<Vector2D<double>, 4>{Line<T>(v[0], v[3]).normal(), Line<T>(v[1], v[0]).normal(),
                                               Line<T>(v[2], v[1]).normal(), Line<T>(v[3], v[2]).normal()};
    }
    [[nodiscard]] constexpr virtual auto center() const -> Position<T> override
    {
        return Position{(unrotatedLeft() + unrotatedRight()) / 2, (unrotatedTop() + unrotatedBottom()) / 2};
    };
    [[nodiscard]] constexpr auto contains(Position<T> pos) -> bool
    {
        Position<T> rotated = m_rotationAroundOriginMatrix.reverseRotation() * pos;
        return !(rotated.y < unrotatedTop() || rotated.y > unrotatedBottom() || rotated.x < unrotatedLeft() ||
                 rotated.x > unrotatedRight());
    };
    [[nodiscard]] constexpr auto pos() const -> Position<T> { return m_rotationAroundCenterMatrix * unrotatedPos; };
    [[nodiscard]] constexpr auto width() const -> T { return m_size.width; };
    [[nodiscard]] constexpr auto height() const -> T { return m_size.height; };
    constexpr void rotate(double angleRadians)
    {
        orientation += angleRadians;
        while (orientation > std::numbers::pi / 2)
            orientation -= std::numbers::pi;
        while (orientation <= -std::numbers::pi / 2)
            orientation += std::numbers::pi;
        m_rotationAroundOriginMatrix = calculateRotationAroundOriginMatrix(orientation);
        m_rotationAroundCenterMatrix =
            calculateRotationAroundCenterMatrix(m_rotationAroundOriginMatrix, m_translationToOriginMatrix);
        auto rotatedPos = m_rotationAroundCenterMatrix * unrotatedPos;
    };
    [[nodiscard]] constexpr auto size() const -> Size<T> { return m_size; };
    constexpr void translate(const Vector2D<double>& translation)
    {
        unrotatedPos += translation;
        m_translationToOriginMatrix = calculateTranslationToOriginMatrix(center());
    }
    [[nodiscard]] constexpr auto getDrawables() const -> std::vector<Triangle<T>>
    {
        Quadrilateral<T> rec{vertices()};
        return rec.getTriangles();
    }
    [[nodiscard]] constexpr auto bottom() const -> T
    {
        T max = 0;
        return findExtremeHelper([&max](Position<T> pos) -> bool {
                   if (pos.y > max)
                   {
                       max = pos.y;
                       return true;
                   }
                   return false;
               })
            .y;
    }
    [[nodiscard]] constexpr auto top() const -> T
    {
        T min = static_cast<T>(INT_MAX);
        return findExtremeHelper([&min](Position<T> pos) -> bool {
                   if (pos.y < min)
                   {
                       min = pos.y;
                       return true;
                   }
                   return false;
               })
            .y;
    }
    [[nodiscard]] constexpr auto left() const -> T
    {
        T min = static_cast<T>(INT_MAX);
        return findExtremeHelper([&min](Position<T> pos) {
                   if (pos.x < min)
                   {
                       min = pos.x;
                       return true;
                   }
                   return false;
               })
            .x;
    }
    [[nodiscard]] constexpr auto right() const -> int
    {
        T max = 0;
        return findExtremeHelper([&max](Position<T> pos) {
                   if (pos.x > max)
                   {
                       max = pos.x;
                       return true;
                   }
                   return false;
               })
            .x;
    }
    [[nodiscard]] constexpr auto verticesCCW() const
    {
        return std::array<Position<T>, 4>{topLeft(), bottomLeft(), bottomRight(), topRight()};
    }
    [[nodiscard]] constexpr auto verticesCW() const
    {
        return std::array<Position<T>, 4>{topLeft(), topRight(), bottomRight(), bottomLeft()};
    }

  private:
    [[nodiscard]] constexpr auto findExtremeHelper(std::function<bool(Position<T>)> predicate) const -> Position<T>
    {
        auto corners = vertices();
        int corner = 0;
        for (int i = 0; i < corners.size(); i++)
        {
            if (predicate(corners[i]))
            {
                corner = i;
            }
        }
        return corners[corner];
    }
    [[nodiscard]] constexpr virtual auto vertices() const -> std::array<Position<T>, 4> override
    {
        return std::array<Position<T>, 4>{topLeft(), topRight(), bottomRight(), bottomLeft()};
    }
    [[nodiscard]] constexpr auto topLeft() const -> Position<T> { return pos(); };
    [[nodiscard]] constexpr auto topRight() const -> Position<T>
    {
        return m_rotationAroundCenterMatrix * unrotatedTopRight();
    };
    [[nodiscard]] constexpr auto bottomLeft() const -> Position<T>
    {
        return m_rotationAroundCenterMatrix * unrotatedBottomLeft();
    };
    [[nodiscard]] constexpr auto bottomRight() const -> Position<T>
    {
        return m_rotationAroundCenterMatrix * unrotatedBottomRight();
    };
    [[nodiscard]] constexpr auto unrotatedLeft() const -> T { return unrotatedPos.x; };
    [[nodiscard]] constexpr auto unrotatedRight() const -> T { return unrotatedPos.x + m_size.width; };
    [[nodiscard]] constexpr auto unrotatedTop() const -> T { return unrotatedPos.y; };
    [[nodiscard]] constexpr auto unrotatedBottom() const -> T { return unrotatedPos.y + m_size.height; };
    [[nodiscard]] constexpr auto unrotatedTopLeft() const -> Position<T> { return unrotatedPos; };
    [[nodiscard]] constexpr auto unrotatedTopRight() const -> Position<T>
    {
        return unrotatedPos + Position<T>(m_size.width, 0);
    };
    [[nodiscard]] constexpr auto unrotatedBottomLeft() const -> Position<T>
    {
        return unrotatedPos + Position<T>(0, m_size.height);
    };
    [[nodiscard]] constexpr auto unrotatedBottomRight() const -> Position<T>
    {
        return unrotatedPos + Position<T>(m_size.width, m_size.height);
    };
    [[nodiscard]] constexpr static auto calculateRotationAroundOriginMatrix(double orientation) -> Matrix3X3<double>
    {
        return Matrix3X3<double>::fromRotation(orientation);
    };
    [[nodiscard]] constexpr static auto calculateTranslationToOriginMatrix(Position<T>&& center) -> Matrix3X3<double>
    {
        return Matrix3X3<double>::fromTranslation(-center);
    };
    [[nodiscard]] constexpr static auto
    calculateRotationAroundCenterMatrix(Matrix3X3<double> rotationAroundOriginMatrix,
                                        Matrix3X3<double> translationToOriginMatrix) -> Matrix3X3<double>
    {
        Matrix3X3 intermediateMatrix = rotationAroundOriginMatrix * translationToOriginMatrix;
        Matrix3X3 secondMatrix = translationToOriginMatrix.reverseTranslation();
        Matrix3X3 finalMatrix = secondMatrix * intermediateMatrix;
        return translationToOriginMatrix.reverseTranslation() *
               (rotationAroundOriginMatrix * translationToOriginMatrix);
    };

    Position<T> unrotatedPos;
    Size<T> m_size;
    double orientation;
    Matrix3X3<double> m_rotationAroundOriginMatrix;
    Matrix3X3<double> m_translationToOriginMatrix;
    Matrix3X3<double> m_rotationAroundCenterMatrix;
};

#endif // RECTANGLE_H
