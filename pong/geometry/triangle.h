#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "line.h"
#include "vector.h"

#include <algorithm>
#include <array>
template <typename T>
class Triangle
{
  public:
    constexpr Triangle(Position<T> a, Position<T> b, Position<T> c);
    constexpr auto getVertices() const { return m_vertices; }
    constexpr static auto decomposeTriangle(Triangle triangle) -> std::pair<Triangle, Triangle>;

    // A triangle is decomposable if it doesn't have a flat, horizontal top or
    // bottom line. It can then be decomposed into two triangles that each have
    // a flat, horizontal top or bottom line. This decomposed form is more
    // suitable for rendering
    constexpr bool isDecomposable() const;

  private:
    std::array<Position<T>, 3> m_vertices;
};

template <typename T>
constexpr Triangle<T>::Triangle(Position<T> a, Position<T> b, Position<T> c) : m_vertices{a, b, c}
{
    std::sort(m_vertices.begin(), m_vertices.end(), [](const Position<T>& a, const Position<T>& b) {
        if (a.y == b.y)
        {
            return a.x < b.x;
        }
        return a.y < b.y;
    });
}
template <typename T>
constexpr auto Triangle<T>::decomposeTriangle(Triangle<T> triangle) -> std::pair<Triangle, Triangle>
{
    auto vertices = triangle.getVertices();
    Line line{vertices[0], vertices[2]};
    Position newVertex{line.xFromY(vertices[1].y), vertices[1].y};
    return std::make_pair(Triangle(vertices[0], vertices[1], newVertex), Triangle(vertices[1], newVertex, vertices[2]));
}

template <typename T>
constexpr auto Triangle<T>::isDecomposable() const -> bool
{
    return !(m_vertices[0].y == m_vertices[1].y || m_vertices[1].y == m_vertices[2].y);
}
#endif // TRIANGLE_H
