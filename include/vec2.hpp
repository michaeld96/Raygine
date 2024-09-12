#ifndef VEC2_HPP
#define VEC2_HPP
#include <cmath>
template <typename T>
class Vec2
{
public:
    T x;
    T y;

    Vec2<T>& operator+(const Vec2<T>& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vec2<T>& operator*(const T& scalar)
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }
    Vec2<T>& operator*(const Vec2<T>& other)
    {
        x *= other.x;
        y *= other.y;
        return *this;
    }

    bool operator==(const Vec2<T>& other) const // const after because we are not modifying the object.
    {
        return x == other.x && y == other.y;
    }

    Vec2<T>& operator*=(const T& scalar)
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    Vec2<T>& operator*=(const Vec2<T>& other)
    {
        x *= other.x;
        y *= other.y;
        return *this;
    }
};

template <typename T>
Vec2<T> RotationMatrix2D(Vec2<T> vec, float theta)
{
    T x = vec.x;
    T y = vec.y;
    T x_prime = x * std::cos(theta) - y * std::sin(theta);
    T y_prime = x * std::sin(theta) + y * std::cos(theta);
    return { x_prime, y_prime };
}
#endif  // VEC2_HPP