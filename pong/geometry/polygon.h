#ifndef POLYGON_H
#define POLYGON_H
template <typename T, size_t S>
class ConvexPolygon
{
  public:
    virtual [[nodiscard]] auto lines() const -> std::array<Line<T>, S> = 0;
    virtual [[nodiscard]] auto vertices() const -> std::array<Position<T>, S> = 0;
    virtual [[nodiscard]] auto normals() const -> std::array<Vector2D<double>, S> = 0;
    virtual [[nodiscard]] auto center() const -> Position<T> = 0;
};

#endif // POLYGON_H
