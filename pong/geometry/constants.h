#ifndef CONSTANTS_H
#define CONSTANTS_H
#include "matrix.h"
#include "vector.h"
namespace constants
{
template <typename T>
constexpr Matrix3X3<T> PiHalfRotationMatrix = Matrix3X3<T>(0, -1, 1, 0);
template <typename T> constexpr Vector2D<T> XUnitVector = Vector2D<T>(1, 0);
template <typename T> constexpr Vector2D<T> YUnitVector = Vector2D<T>(0, 1);

}; // namespace constants

#endif // CONSTANTS_H
