#include <utility>
#include <list>
#include <functional>
#include <string>
#include <memory>
#include <stdexcept>
#include <iostream>

#define RST  "\x1B[0m"
#define CGREEN  "\x1B[32m"
#define CWHITE  "\x1B[0m"
#define CRED  "\x1B[31m"
#define BOLD(x) "\x1B[1m" x RST

#define ASSERT_EQUAL(a, b) test::assert_equal_impl(a, b, #a, #b,  __LINE__, __FILE__);

#define IGNORE_TEST_CASE(TC) void ignore_tc_##TC()






#define TEST_CASE(TC) namespace test { struct Test_##TC               \
{                                                   \
    void execute();                                 \
                                                    \
    void registerMe()                               \
    {                                               \
        TestRunner::instance().registerTest(        \
                     #TC,                           \
                     std::bind(&Test_##TC::execute, this)   \
                );                                  \
    }                                               \
                                                    \
    Test_##TC()                                          \
    {                                               \
        registerMe();                               \
    }                                               \
};                                                  \
} \
static test::Test_##TC tc_##TC;                                     \
void test::Test_##TC::execute()                                 \

namespace test
{
    template <typename A, typename B>
    void assert_equal_impl(const A& a, const B& b,
            const char* a_label,
            const char* b_label,
            int line_number,
            const char* filename)
    {
        if (!(a == b)){
            std::cout << "\x1B[1m\x1B[31m[ERROR] Assertion failed at line " <<filename << ":" << line_number
                      << ": \x1B[0m\n    expression: [ " << a_label << " != " << b_label
                      << " ]\n    values: [ " << a << " != " << b  << " ] " << std::endl;
            throw new std::runtime_error("fail");
        }
    }

    struct TestRunner
    {
        static TestRunner& instance()
        {
            static TestRunner instance;
            return instance;
        }

        void registerTest(const std::string& label, const std::function<void()>& tc)
        {
            _tests.push_back(std::make_pair(label, tc));
        }

        void runTests()
        {
            for (const auto& tc: _tests) {
                std::cout << CGREEN << BOLD("[START ] ") << tc.first << "" << CWHITE << std::endl;
                try {
                    tc.second();

                    std::cout << CGREEN << BOLD("[PASSED] ") << tc.first << "" << CWHITE << std::endl;
                    std::cout << std::endl;
                } catch(...) {
                    std::cout << CRED << BOLD("[FAILED] ") << tc.first << "" << CWHITE << std::endl;
                    std::cout << std::endl;
                }
            }
        }
        std::list<std::pair<std::string, std::function<void()>>>  _tests;
    };
}

int main()
{
    test::TestRunner::instance().runTests();
}
