#ifndef POLYGON_H
#define POLYGON_H
template <typename T, size_t S>
class ConvexPolygon
{
  public:
    virtual [[nodiscard]] std::array<Line<T>, S> lines() const = 0;
    virtual [[nodiscard]] std::array<Position<T>, S> vertices() const = 0;
    virtual [[nodiscard]] std::array<Vector2D<double>, S> normals() const = 0;
    virtual [[nodiscard]] Position<T> center() const = 0;
};

#endif // POLYGON_H
