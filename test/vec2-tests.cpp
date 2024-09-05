#include "../include/vec2.hpp" 
#include "test-macros.hpp"

bool test_rotation()
{
    std::cout << "Testing 2D Vector Rotation...\n";
    Vec2<float> v1 = { 1.0f, 2.0f };
    v1 = RotationMatrix2D(v1, (3.1415926535 / 2));
    ASSERT_EQUAL(v1.x, (float)-2, 0.001f);
    ASSERT_EQUAL(v1.y, (float)1, 0.001f);
    std::cout << "All 2D Vector Rotation Tests passed!\n";
    return true;
}

int main()
{
    test_rotation();
    std::cout << "Ran all tests! :)\n";
    return 0;
}