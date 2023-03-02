#ifndef RECTANGLE_H
#define RECTANGLE_H
#include "constants.h"
#include "line.h"
#include "matrix.h"
#include "quadrilateral.h"
#include "triangle.h"
#include "vector.h"

#include <algorithm>
#include <array>
#include <numbers>
template <typename T> struct Size
{
    T width;
    T height;
};
template <typename T> class Rectangle
{
  public:
    constexpr Rectangle(T x, T y, T w, T h)
        : unrotatedPos{x, y}, m_size{w, h}, orientation{0},
          m_rotationAroundOriginMatrix{
              calculateRotationAroundOriginMatrix(orientation)},
          m_translationToOriginMatrix{
              calculateTranslationToOriginMatrix(center())},
          m_rotationAroundCenterMatrix{calculateRotationAroundCenterMatrix(
              m_rotationAroundOriginMatrix, m_translationToOriginMatrix)}
    {
    }
    constexpr Rectangle(Position<T> p, T w, T h)
        : unrotatedPos{p}, m_size{w, h}, orientation{0},
          m_rotationAroundOriginMatrix{
              calculateRotationAroundOriginMatrix(orientation)},
          m_translationToOriginMatrix{
              calculateTranslationToOriginMatrix(center())},
          m_rotationAroundCenterMatrix{calculateRotationAroundCenterMatrix(
              m_rotationAroundOriginMatrix, m_translationToOriginMatrix)}
    {
    }
    constexpr Rectangle(T x, T y, Size<T> s)
        : unrotatedPos{x, y}, m_size{s}, orientation{0},
          m_rotationAroundOriginMatrix{
              calculateRotationAroundOriginMatrix(orientation)},
          m_translationToOriginMatrix{
              calculateTranslationToOriginMatrix(center())},
          m_rotationAroundCenterMatrix{calculateRotationAroundCenterMatrix(
              m_rotationAroundOriginMatrix, m_translationToOriginMatrix)}
    {
    }
    [[nodiscard]] std::array<Line<T>, 4> lines() const
    {
        auto lines = std::array<Line, 4>{
            Line(topLeft(),
                 m_rotationAroundOriginMatrix * constants::XUnitVector<double>,
                 static_cast<double>(m_size.width)),
            Line(topRight(),
                 m_rotationAroundOriginMatrix * constants::YUnitVector<double>,
                 static_cast<double>(m_size.height)),
            Line(bottomRight(),
                 m_rotationAroundOriginMatrix * constants::XUnitVector<double>,
                 static_cast<double>(m_size.width)),
            Line(bottomLeft(),
                 m_rotationAroundOriginMatrix * constants::YUnitVector<double>,
                 static_cast<double>(m_size.height))};
        std::ranges::sort(lines, [](Line a, Line b) {
            return a.highestPoint() > b.highestPoint();
        });
        return lines;
    };
    // [[nodiscard]] constexpr int left() const { return pos().x; };
    // [[nodiscard]] constexpr int right() const { return pos().x + size.width;
    // };
    // [[nodiscard]] constexpr int top() const { return pos().y; };
    // [[nodiscard]] constexpr int bottom() const
    // {
    //     return pos().y + size.height;
    // };
    [[nodiscard]] constexpr Position<T> topLeft() const { return pos(); };
    [[nodiscard]] constexpr Position<T> topRight() const
    {
        return m_rotationAroundCenterMatrix * unrotatedTopRight();
    };
    [[nodiscard]] constexpr Position<T> bottomLeft() const
    {
        return m_rotationAroundCenterMatrix * unrotatedBottomLeft();
    };
    [[nodiscard]] constexpr Position<T> bottomRight() const
    {
        return m_rotationAroundCenterMatrix * unrotatedBottomRight();
    };
    [[nodiscard]] constexpr Position<T> center() const
    {
        return Position{(unrotatedLeft() + unrotatedRight()) / 2,
                        (unrotatedTop() + unrotatedBottom()) / 2};
    };
    [[nodiscard]] constexpr bool contains(Position<T> pos)
    {
        Position<T> rotated =
            m_rotationAroundOriginMatrix.reverseRotation() * pos;
        return !(rotated.y < unrotatedTop() || rotated.y > unrotatedBottom() ||
                 rotated.x < unrotatedLeft() || rotated.x > unrotatedRight());
    };
    [[nodiscard]] constexpr Position<T> pos() const
    {
        return m_rotationAroundCenterMatrix * unrotatedPos;
    };
    [[nodiscard]] constexpr T width() const { return m_size.width; };
    [[nodiscard]] constexpr T height() const { return m_size.height; };
    constexpr void rotate(double angleRadians)
    {
        orientation += angleRadians;
        while (orientation > std::numbers::pi / 2)
            orientation -= std::numbers::pi;
        while (orientation <= -std::numbers::pi / 2)
            orientation += std::numbers::pi;
        m_rotationAroundOriginMatrix =
            calculateRotationAroundOriginMatrix(orientation);
        m_rotationAroundCenterMatrix = calculateRotationAroundCenterMatrix(
            m_rotationAroundOriginMatrix, m_translationToOriginMatrix);
        auto rotatedPos = m_rotationAroundCenterMatrix * unrotatedPos;
    };
    [[nodiscard]] constexpr Size<T> size() const { return m_size; };
    constexpr void translate(const Vector2D<float>& translation)
    {
        unrotatedPos += translation;
        m_translationToOriginMatrix =
            calculateTranslationToOriginMatrix(center());
    }
    [[nodiscard]] constexpr std::vector<Triangle<T>> getDrawables() const
    {
        Quadrilateral<T> rec{getCorners()};
        return rec.getTriangles();
    }

  private:
    [[nodiscard]] constexpr T unrotatedLeft() const { return unrotatedPos.x; };
    [[nodiscard]] constexpr std::array<Position<T>, 4> getCorners() const
    {
        return std::array<Position<T>, 4>{topLeft(), topRight(), bottomRight(),
                                          bottomLeft()};
    }
    [[nodiscard]] constexpr T unrotatedRight() const
    {
        return unrotatedPos.x + m_size.width;
    };
    [[nodiscard]] constexpr T unrotatedTop() const { return unrotatedPos.y; };
    [[nodiscard]] constexpr T unrotatedBottom() const
    {
        return unrotatedPos.y + m_size.height;
    };
    [[nodiscard]] constexpr Position<T> unrotatedTopLeft() const
    {
        return unrotatedPos;
    };
    [[nodiscard]] constexpr Position<T> unrotatedTopRight() const
    {
        return unrotatedPos + Position<T>(m_size.width, 0);
    };
    [[nodiscard]] constexpr Position<T> unrotatedBottomLeft() const
    {
        return unrotatedPos + Position<T>(0, m_size.height);
    };
    [[nodiscard]] constexpr Position<T> unrotatedBottomRight() const
    {
        return unrotatedPos + Position<T>(m_size.width, m_size.height);
    };
    [[nodiscard]] constexpr static Matrix3X3<double>
    calculateRotationAroundOriginMatrix(double orientation)
    {
        return Matrix3X3<double>::fromRotation(orientation);
    };
    [[nodiscard]] constexpr static Matrix3X3<double>
    calculateTranslationToOriginMatrix(Position<T>&& center)
    {
        return Matrix3X3<double>::fromTranslation(-center);
    };
    [[nodiscard]] constexpr static Matrix3X3<double>
    calculateRotationAroundCenterMatrix(
        Matrix3X3<double> rotationAroundOriginMatrix,
        Matrix3X3<double> translationToOriginMatrix)
    {
        Matrix3X3 intermediateMatrix =
            rotationAroundOriginMatrix * translationToOriginMatrix;
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
