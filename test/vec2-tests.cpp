#include "../include/vec2.hpp" 
#include "test-macros.hpp"

void test_rotation()
{
    Vec2 v = { 1, 2 };
    v = RotationMatrix2D(v, (3.1415926535 / 2));
    ASSERT_EQUAL(v.x, (float)-2, 0.001f);
    ASSERT_EQUAL(v.y, (float)1, 0.001f);
}

int main()
{
    test_rotation();
    ASSERT_EQUAL(1, 1, 0);
    std::cout << "Ran all tests! :)\n";
    return 0;
}