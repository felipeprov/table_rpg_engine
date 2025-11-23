#pragma once

#include <iostream>
#include <string>
#include <vector>

namespace test_framework {
    inline int failures = 0;
    
    inline void assert_true(bool condition, const char* expr, const char* file, int line) {
        if (!condition) {
            std::cerr << "Assertion failed: " << expr << " at " << file << ":" << line << std::endl;
            ++failures;
        }
    }
}

#define ASSERT_TRUE(expr) test_framework::assert_true((expr), #expr, __FILE__, __LINE__)

#define SETUP(block) static void test_framework_setup() { \
    do block while(0); \
}

#define TEARDOWN(block) static void test_framework_teardown() { \
    do block while(0); \
}

struct TestCase {
    std::string name;
    void (*fn)();
};

inline std::vector<TestCase>& registry() {
    static std::vector<TestCase> r;
    return r;
}

struct Registrar {
  Registrar(const char *name, void (*fn)()) {
    registry().push_back(TestCase{name, fn});
  }
};

// Macro to define the main function for tests
#define TEST_MAIN(block) int main() {       \
    do block while(0);                       \
    if (test_framework::failures == 0) { \
        std::cout << "All tests passed!" << std::endl; \
        return 0; \
    } else { \
        std::cout << test_framework::failures << " tests failed." << std::endl; \
        return 1; \
    } \
}

#define TEST_CASE(name) \
    void name(); \
    static Registrar _reg_##name(#name, &name); \
    void name()

#define TESTF(func_name) {  \
    test_framework_setup(); \
    func_name(); \
    test_framework_teardown(); \
}

#define TESTB(block) {  \
    test_framework_setup(); \
    do block while(0); \
    test_framework_teardown(); \
}