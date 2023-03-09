#ifndef QUADRILATERAL_H
#define QUADRILATERAL_H

#include "triangle.h"
#include "vector.h"

#include <algorithm>
#include <vector>
template <typename T>
class Quadrilateral
{
  public:
    constexpr Quadrilateral(Position<T> a, Position<T> b, Position<T> c, Position<T> d) : m_corners{a, b, c, d}
    {
        sortCorners();
    }
    constexpr Quadrilateral(std::array<Position<T>, 4> corners) : m_corners{corners} { sortCorners(); }
    [[nodiscard]] constexpr auto getTriangles() const -> std::vector<Triangle<T>>
    {
        std::vector<Triangle<T>> triangles{};
        triangles.reserve(4);
        auto tri0 = Triangle<T>(m_corners[0], m_corners[1], m_corners[2]);
        auto tri1 = Triangle<T>(m_corners[1], m_corners[2], m_corners[3]);
        if (tri0.isDecomposable())
        {
            auto [first, second] = Triangle<T>::decomposeTriangle(tri0);
            triangles.push_back(first);
            triangles.push_back(second);
        }
        else
        {
            triangles.push_back(tri0);
        }
        if (tri1.isDecomposable())
        {
            auto [first, second] = Triangle<T>::decomposeTriangle(tri1);
            triangles.push_back(first);
            triangles.push_back(second);
        }
        else
        {
            triangles.push_back(tri1);
        }
        return triangles;
    }
    [[nodiscard]] constexpr auto getVertices() const { return m_corners; }

  private:
    constexpr void sortCorners()
    {
        std::sort(m_corners.begin(), m_corners.end(), [](const Position<T>& a, const Position<T>& b) -> bool {
            if (a.y == b.y)
            {
                return a.x < b.x;
            }
            return a.y < b.y;
        });
    }

    std::array<Position<T>, 4> m_corners;
};

#endif // QUADRILATERAL_H
