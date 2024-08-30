#include "../include/vec2.hpp"

Vec2 RotationMatrix2D(Vec2 vec, float theta)
{
    float x = vec.x;
    float y = vec.y;
    float x_prime = x * cos(theta) - y * sin(theta);
    float y_prime = x * sin(theta) + y * cos(theta);
    return { x_prime, y_prime };
}