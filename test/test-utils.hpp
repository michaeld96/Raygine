#ifndef TEST_UTILS_HPP
#define TEST_UTILS_HPP

#include <cmath>  // Used for std::fabs.
#include <functional> // Used for std::function.
#include <iostream>
#include <sstream> // ostringstream, converting << into a string object.
#include <string>
#include <unordered_map>
#include <vector>

/**
 * This header only file is a custom testing framework where it uses a hashmap
 * for all the function names and then uses function pointers to call those functions
 * and see if they are passing their tests or not.
 * 
 * Output is red if there is an error with the input or a test failed. Output is 
 * green if the test passed.
 * 
 * Every failed test has information dumped into the console 
 * of: what the name of the test was that failed,
 * what line the test failed on, where this file is located, and what
 * condition the test failed. Passing information is just the status of the test,
 * which is SUCCESS, and then the name of the test.
 * 
 * Every function that is displayed in the console is in single quotations, so, for 
 * example, the function foo will be represented as `foo` in the console.
 * 
 */

namespace TestUtils
{

/* DATA STRUCTURES */
enum StringOutputColor 
{
    RED,
    GREEN
};

std::unordered_map<std::string, std::function<bool()>> FunctionNameAndPointers;

/* FUNCTIONS */
void TestOutput(const std::string& message, StringOutputColor color)
{
    const std::string RED_COLOR = "\033[31m";
    const std::string GREEN_COLOR = "\033[32m";
    const std::string RESET_COLOR = "\033[0m";

    switch (color)
    {
        case StringOutputColor::RED:
        {
            std::cout << RED_COLOR << message << RESET_COLOR << "\n";
            break;
        }
        case StringOutputColor::GREEN:
        {
            std::cout << GREEN_COLOR << message << RESET_COLOR << "\n";
            break;
        }
        default:
        {
            std::cout << message << "\n";
            break;
        }
    }
}
/**
 * @brief Loads function names into a hash map. If a duplicate function name is found,
 *        an error message is printed and the loading process is stopped.
 * 
 * @param func_names A pointer to an array of function names to be loaded.
 * @param size The size of the array.
 */
void LoadFunctions(const std::vector<std::string> func_names, const std::vector<std::function<bool()>> func_ptrs)
{
    for (size_t i = 0; i < func_names.size(); ++i)
    {
        if (FunctionNameAndPointers.find(func_names[i]) == FunctionNameAndPointers.end())
        {
            FunctionNameAndPointers.insert( {func_names[i], func_ptrs[i]} );
        }
        else
        {
            TestOutput("ERROR: Duplicate function name: " + func_names[i]
                            + " already exists", StringOutputColor::RED);
            break;
        }
    }
}

/**
 * @brief Runs test on the given function name. If the function name is not present
 *        this function will throw an error.
 * 
 * @param func_name The name of the function you want to run.
 * 
 */
void RunFunction(std::string func_name)
{
    if (FunctionNameAndPointers.find(func_name) != FunctionNameAndPointers.end())
    {
        // Run the function.
        std::cout << "Running test on '" << func_name << "'...\n";
        auto _func_ptr_itr = FunctionNameAndPointers.find(func_name);
        bool result =_func_ptr_itr->second();
        std::cout << "Done running test on '" << func_name << "'\n";
    }
    else
    {
        TestOutput("ERROR: Function '" + func_name + "' was not found.", StringOutputColor::RED);
    }
}

void RunAllFunctions(void)
{
    for (auto& pair : FunctionNameAndPointers)
    {
        // Get the function name and run it in RunFunction.
        RunFunction(pair.first);
    }
}

/* MACROS */
#define ASSERT_TRUE(condition) do {                                 \
    std::ostringstream oss;                                         \
    if (!(condition)) {                                             \
        oss << "FAIL: Test failed for function: " << __func__ <<    \
        "'" <<                                                      \
        "\nFile: " << __FILE__ << "\nLine: " << __LINE__            \
        << "\nCondition " << #condition << " is false";             \
        TestOutput(oss.str(), TestUtils::StringOutputColor::RED);   \
        return false;                                               \
    }                                                               \
    else {                                                          \
        oss << "SUCCESS: Test passed for function: " << __func__    \
        << "'";                                                     \
        TestOutput(oss.str(), TestUtils::StringOutputColor::GREEN); \
    }                                                               \
} while (0)

#define ASSERT_EQUAL(cond_1, cond_2) do {                           \
    std::ostringstream oss;                                         \
    if (!(cond_1 == cond_2)) {                                      \
        oss << "FAIL: Test failed for function: '" << __func__ <<   \
        "'" <<                                                      \
        "\nFile: " << __FILE__ << "\nLine: " << __LINE__ << "\n"    \
        << #cond_1 << " != " << #cond_2;                            \
        TestOutput(oss.str(), TestUtils::StringOutputColor::RED);   \
        return false;                                               \
    }                                                               \
    else {                                                          \
        oss << "SUCCESS: Test passed for function '" << __func__    \
        << "'";                                                     \
        TestOutput(oss.str(), TestUtils::StringOutputColor::GREEN); \
    }                                                               \
} while (0)





} // namespace TestUtils

#endif // TEST_UTILS_HPP