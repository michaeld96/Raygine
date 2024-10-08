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
 * Still toying with the idea if I want this to be an object or not... :|
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
std::unordered_map<std::string, std::function<void()>> FunctionNameAndPointers;
/* CONSTANTS */
const char * _header_footer = "**********************************\n";
const char * _test_summary_header = "********** Test Summary **********\n";

/* VARIABLES */
int _num_passed;
int _num_failed;

bool _running_all_tests = false;
int _total_num_passed;
int _total_num_failed;

/* FUNCTIONS */

/**
 * @brief \b ONLY \b USED \b FOR \b INTERNAL \b USE!
 * This function "renders" testing output with a specific color. Each of these colors
 * is specified with the `StringOutputColor` which can be `RED` or `GREEN`.
 * @param message A string that you want to be displayed to the console.
 * @param color Color can be either `StringOutputColor::RED` or `StringOutputColor::GREEN`.
 * 
 */
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
 * @brief \b ONLY \b USED \b FOR \b INTERNAL \b USE!
 *        Prints the number of passed and failed asserts that were
 *        present in the test. Will print out green for passed tests
 *        and red for failed tests.
 */
void PrintTestSummary(void)
{
    TestOutput("Test(s) Passed: " + std::to_string(_num_passed),
                                            StringOutputColor::GREEN);
    TestOutput("Test(s) Failed: " + std::to_string(_num_failed), 
                                            StringOutputColor::RED);
}
/**
 * @brief \b ONLY \b USED \b FOR \b INTERNAL \b USE!
 *        Prints the total number of tests that passed or failed. This function will 
 *        only be ran if `RunAllTest()` is called.
 */
void PrintTotalTestSummary(void)
{
    std::cout <<_test_summary_header;
    TestOutput("Total Test(s) Passed: " + std::to_string(_total_num_passed),
                                                StringOutputColor::GREEN);
    TestOutput("Total Test(s) Failed: " + std::to_string(_total_num_failed), 
                                                StringOutputColor::RED);
    std::cout << _header_footer;
}
/**
 * @brief Loads function names into a hash map. If a duplicate function name is found,
 *        an error message is printed and the loading process is stopped.
 * 
 * @param func_names A pointer to an array of function names to be loaded.
 * @param size The size of the array.
 */
void LoadFunctions(const std::vector<std::string> func_names, const std::vector<std::function<void()>> func_ptrs)
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
 * @brief Specify which function to remove from the testing environment. If this
 *        function is not found, then an error will be dumped to the console.
 * @param func_name The name of the function which would be remove.
 */
void RemoveFunction(const std::string func_name)
{
    if (FunctionNameAndPointers.find(func_name) != FunctionNameAndPointers.end())
    {
        FunctionNameAndPointers.erase(func_name);
    }
    else
    {
        TestOutput("ERROR: Function '" + func_name + "' was not found.", StringOutputColor::RED);
    }
}

/**
 * @brief Will remove all functions from the testing environment.
 */
void RemoveAllFunctions()
{
    FunctionNameAndPointers.clear();
}

/**
 * @brief Runs test on the given function name. If the function name is not present
 *        this function will throw an error.
 * 
 * @param func_name The name of the function you want to run.
 * 
 */
void RunTest(std::string func_name)
{
    std::cout << _header_footer;
    _num_passed = 0;
    _num_failed = 0;
    if (FunctionNameAndPointers.find(func_name) != FunctionNameAndPointers.end())
    {
        // Run the function.
        std::cout << "Running test on '" << func_name << "'...\n";
        auto _func_ptr_itr = FunctionNameAndPointers.find(func_name);
        _func_ptr_itr->second();
        std::cout << "Done running test on '" << func_name << "'\n";
    }
    else
    {
        TestOutput("ERROR: Function '" + func_name + "' was not found.", StringOutputColor::RED);
    }
    if (_running_all_tests)
    {
        _total_num_passed += _num_passed;
        _total_num_failed += _num_failed;
    }
    PrintTestSummary();

}

/**
 * @brief This function will run all tests and prints a summary of all the 
 *        tests that were ran (showing the number of passing and failing tests).
 */
void RunAllTests(void)
{
    _running_all_tests = true;
    for (auto& pair : FunctionNameAndPointers)
    {
        // Get the function name and run it in RunFunction.
        RunTest(pair.first);
    }
    std::cout << _header_footer;
    PrintTotalTestSummary();
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
        TestUtils::_num_failed++;                                   \
    }                                                               \
    else {                                                          \
        oss << "SUCCESS: Test passed for function: " << __func__    \
        << "'";                                                     \
        TestOutput(oss.str(), TestUtils::StringOutputColor::GREEN); \
        TestUtils::_num_passed++;                                   \
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
        TestUtils::_num_failed++;                                   \
    }                                                               \
    else {                                                          \
        oss << "SUCCESS: Test passed for function '" << __func__    \
        << "'";                                                     \
        TestOutput(oss.str(), TestUtils::StringOutputColor::GREEN); \
        TestUtils::_num_passed++;                                   \
    }                                                               \
} while (0)





} // namespace TestUtils

#endif // TEST_UTILS_HPP