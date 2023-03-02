#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "line.h"
#include "vector.h"

#include <algorithm>
#include <array>
template <typename T> class Triangle
{
  public:
    constexpr Triangle(Position<T> a, Position<T> b, Position<T> c)
        : m_vertices{a, b, c}
    {
        std::sort(m_vertices.begin(), m_vertices.end(),
                  [](const Position<T>& a, const Position<T>& b) {
                      if (a.y == b.y)
                      {
                          return a.x < b.x;
                      }
                      return a.y < b.y;
                  });
    }
    constexpr std::array<Position<T>, 3> getVertices() const
    {
        return m_vertices;
    }
    constexpr static std::pair<Triangle, Triangle>
    decomposeTriangle(Triangle triangle)
    {
        auto vertices = triangle.getVertices();
        Line line{vertices[0], vertices[2]};
        Position newVertex{line.xFromY(vertices[1].y), vertices[1].y};
        return std::make_pair(Triangle(vertices[0], vertices[1], newVertex),
                              Triangle(vertices[1], newVertex, vertices[2]));
    }
    constexpr bool isDecomposable() const
    // A triangle is decomposable if it doesn't have a flat, horizontal top or
    // bottom line. It can then be decomposed into two triangles that each have
    // a flat, horizontal top or bottom line. This decomposed form is more
    // suitable for rendering
    {
        return !(m_vertices[0].y == m_vertices[1].y ||
                 m_vertices[1].y == m_vertices[2].y);
    }

  private:
    std::array<Position<T>, 3> m_vertices;
};

#endif // TRIANGLE_H
