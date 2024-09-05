#ifndef TEST_MACROS_HPP
#define TEST_MACROS_HPP

#include <iostream>
#include <cmath>  // Include cmath for std::fabs

#define ASSERT_TRUE(condition) do {                                 \
    if (!(condition)) {                                             \
        std::cerr << "Test failed for function: " << __func__ <<    \
        "\nFile: " << __FILE__ << "\nLine: " << __LINE__            \
        << "\nCondition " << #condition << " is false.\n";          \
        return false;                                               \
    }                                                               \
} while (0)

#define ASSERT_EQUAL(cond_1, cond_2, epsilon) do {                  \
    if (std::fabs((cond_1) - (cond_2)) > (epsilon)) {               \
        std::cerr << "Test failed for function: " << __func__ <<    \
        "\nFile: " << __FILE__ << "\nLine: " << __LINE__ << "\n"    \
        << #cond_1 << " != " << #cond_2 << "\n";                    \
        return false;                                               \
    }                                                               \
} while (0)

#endif // TEST_MACROS_HPP