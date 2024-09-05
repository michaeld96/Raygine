#ifndef VEC2_HPP
#define VEC2_HPP
#include <cmath>
template <typename T>
class Vec2
{
public:
    T x;
    T y;
};

template <typename T>
Vec2<T> RotationMatrix2D(Vec2<T> vec, float theta)
{
    T x = vec.x;
    T y = vec.y;
    T x_prime = x * cos(theta) - y * sin(theta);
    T y_prime = x * sin(theta) + y * cos(theta);
    return { x_prime, y_prime };
}
#endif  // VEC2_HPP