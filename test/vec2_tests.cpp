#include "../include/vec2.hpp" 
#include "test_utils.hpp"
#include <unordered_map>
#include <string>
#include <vector>

void test_rotation()
{
    Vec2<float> v1 = { 1.0f, 2.0f };
    v1 = RotationMatrix2D(v1, (3.1415926535 / 2));
    ASSERT_EQUAL(v1.x, (float)-2);
    ASSERT_EQUAL(v1.y, 0.99999994f);
}

void test_mult_operator()
{
    Vec2<int> v1 = { 1, 3 };
    Vec2<int> v1_check = { 5, 15 };
    v1 = v1 * 5;
    ASSERT_EQUAL(v1, v1_check);
    
    Vec2<int> v2 = { 2, 6 };
    Vec2<int> v3 = { 3, 10 };
    Vec2<int> v2_correct = { 6, 60 };
    Vec2<int> v2_result = v2 * v3;
    ASSERT_EQUAL(v2_result, v2_correct);
}

void test_plus_vec()
{
    Vec2<int> expected = { 6, 8 };
    Vec2<int> u = { 2, 3 };
    Vec2<int> v = { 4, 5 };
    Vec2<int> result = u + v;
    ASSERT_EQUAL(result, expected);
}

int main()
{
    std::vector<std::string> func_names = {
        "test_rotation",
        "test_mult_operator",
        "test_plus_vec"
    };

    std::vector<std::function<void()>> func_ptrs = {
        test_rotation,
        test_mult_operator,
        test_plus_vec
    };
    
    TestUtils::LoadFunctions(func_names, func_ptrs);
    TestUtils::RunAllTests();
    TestUtils::RemoveAllFunctions();
    return 0;
}