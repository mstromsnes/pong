#ifndef MATRIX_H
#define MATRIX_H
#include <cmath>
template <typename T>
struct Matrix3X3
{
    constexpr Matrix3X3<T>(T xx, T xy, T yx, T yy) : xx{xx}, xy{xy}, xz{0}, yx{yx}, yy{yy}, yz{0}, zx{0}, zy{0}, zz{1}
    {
    }
    constexpr Matrix3X3<T>(T xx, T xy, T xz, T yx, T yy, T yz, T zx, T zy, T zz)
        : xx{xx}, xy{xy}, xz{xz}, yx{yx}, yy{yy}, yz{yz}, zx{zx}, zy{zy}, zz{zz}
    {
    }

    T xx;
    T xy;
    T xz;
    T yx;
    T yy;
    T yz;
    T zx;
    T zy;
    T zz;

    constexpr Matrix3X3<T> operator-() const { return Matrix3X3<T>(-xx, -xy, -yx, -yy); };
    template <typename Phi>
    constexpr static auto fromRotation(Phi rotation) -> Matrix3X3<T>
    {
        return Matrix3X3(std::cos(rotation), -std::sin(rotation), std::sin(rotation), std::cos(rotation));
    };
    constexpr static auto fromTranslation(T x, T y) -> Matrix3X3<T> { return Matrix3X3(1, 0, x, 0, 1, y, 0, 0, 1); };
    template <typename vec>
    constexpr static auto fromTranslation(vec&& pos) -> Matrix3X3<T>
    {
        return fromTranslation(pos.x, pos.y);
    };
    constexpr auto reverseTranslation() -> Matrix3X3<T> { return Matrix3X3(xx, xy, -xz, yx, yy, -yz, zx, zy, zz); }
    constexpr auto reverseRotation() -> Matrix3X3<T> { return Matrix3X3(xx, -xy, xz, -yx, yy, -yz, zx, zy, zz); }
};
template <typename T>
constexpr auto operator*(const Matrix3X3<T>& left, const Matrix3X3<T>& right) -> Matrix3X3<T>
{
    return Matrix3X3<T>(left.xx * right.xx + left.xy * right.yx + left.xz * right.zx,
                        left.xx * right.xy + left.xy * right.yy + left.xz * right.zy,
                        left.xx * right.xz + left.xy * right.yz + left.xz * right.zz,

                        left.yx * right.xx + left.yy * right.yx + left.yz * right.zx,
                        left.yx * right.xy + left.yy * right.yy + left.yz * right.zy,
                        left.yx * right.xz + left.yy * right.yz + left.yz * right.zz,

                        left.zx * right.xx + left.zy * right.yx + left.zz * right.zx,
                        left.zx * right.xy + left.zy * right.yy + left.zz * right.zy,
                        left.zx * right.xz + left.zy * right.yz + left.zz * right.zz);
}

#endif // MATRIX_H
