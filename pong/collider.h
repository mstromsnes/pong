#ifndef COLLIDER_H
#define COLLIDER_H

#include "stage.h"

enum class CollisionType { Horizontal, Vertical, Invalid };

template <typename T> class Collider
{
  public:
    constexpr Collider(){};
    virtual ~Collider() = default;
    virtual const Rectangle<T>& getHitbox() const = 0;
    virtual void collide(Vector2D<double>) = 0;
    template <size_t S1, size_t S2>
    constexpr static std::pair<bool, Vector2D<double>>
    overlap(const ConvexPolygon<T, S1>& polygon1,
            const ConvexPolygon<T, S2>& polygon2,
            const Vector2D<double> velocity)
    {
        auto normals1 = polygon1.normals();
        auto normals2 = polygon2.normals();
        std::vector<Vector2D<double>> lineNormals{};
        for (auto normal : normals1)
        {
            if (velocity * normal < 0)
                lineNormals.push_back(normal);
        }
        for (auto normal : normals2)
        {
            bool alreadyAdded = false;
            for (const auto& existingNormal : lineNormals)
            {
                if (normal == existingNormal)
                {
                    alreadyAdded = true;
                    break;
                }
            }
            if (!alreadyAdded && velocity * normal < 0)
                lineNormals.push_back(normal);
        }
        auto vertices1 = polygon1.vertices();
        auto vertices2 = polygon2.vertices();
        auto minimumTranslation = std::numeric_limits<double>::max();
        Vector2D<double> minimumTranslationAxis = constants::NullVector<double>;
        for (const auto& normal : lineNormals)
        {
            auto [overlap, mtv] =
                shadowsOverlap<S1, S2>(normal, vertices1, vertices2);
            if (!overlap || (mtv == 0.0f && minimumTranslation == 0.0f))
                return std::make_pair(false, constants::NullVector<double>);
            if (mtv < minimumTranslation)
            {
                minimumTranslationAxis = normal;
                minimumTranslation = mtv;
            }
        }
        if (Vector2D<double>(polygon2.center() - polygon1.center()) *
                minimumTranslationAxis <
            0)
            minimumTranslationAxis = -minimumTranslationAxis;
        return std::make_pair(
            true,
            minimumTranslationAxis *
                (minimumTranslation + std::numeric_limits<double>::epsilon()));
    }

  private:
    template <size_t S1, size_t S2>
    constexpr static std::pair<bool, double>
    shadowsOverlap(Vector2D<double> axis, std::array<Position<T>, S1> vertices1,
                   std::array<Position<T>, S2> vertices2)
    {
        auto p1min = Vector2D<double>(vertices1[0]) * axis;
        auto p1max = p1min;
        auto p2min = Vector2D<double>(vertices2[0]) * axis;
        auto p2max = p2min;
        for (int i = 1; i < S1; i++)
        {
            p1min = std::min({p1min, Vector2D<double>(vertices1[i]) * axis});
            p1max = std::max({p1max, Vector2D<double>(vertices1[i]) * axis});
        }
        for (int i = 1; i < S2; i++)
        {
            p2min = std::min({p2min, Vector2D<double>(vertices2[i]) * axis});
            p2max = std::max({p2max, Vector2D<double>(vertices2[i]) * axis});
        }
        bool overlap = (p2min <= p1max && p1min <= p2max);
        double right = p2max - p1min;
        double left = p1max - p2min;
        double minimumTranslationVector =
            right < left ? std::abs(right) : std::abs(-left);
        return std::make_pair(overlap, minimumTranslationVector);
    }
};
#endif // COLLIDER_H
