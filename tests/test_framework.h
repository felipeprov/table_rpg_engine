#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <stdbool.h>
#include <stddef.h>

/* =============== Assertions =============== */

extern int test_framework_failures;

void test_framework_assert_true(bool condition,
                                const char *expr,
                                const char *file,
                                int line);

#define ASSERT_TRUE(expr) \
    test_framework_assert_true((expr), #expr, __FILE__, __LINE__)

/* =============== Setup / Teardown =============== */
/* You define these via macros where needed, same as before. */

#define SETUP(block) \
    void test_framework_setup(void) { \
        do block while (0); \
    }

#define TEARDOWN(block) \
    void test_framework_teardown(void) { \
        do block while (0); \
    }

/* If you use TESTF/TESTB, you must also define SETUP/TEARDOWN once. */
void test_framework_setup(void);
void test_framework_teardown(void);

/* =============== Registry =============== */

typedef struct {
    const char *name;
    void (*fn)(void);
} TestCase;

typedef struct {
    TestCase *items;
    size_t count;
    size_t capacity;
} TestRegistry;

/* Returns the global registry */
TestRegistry *test_framework_registry(void);

/* Registers a test case (used by TEST_CASE) */
void test_framework_register(const char *name, void (*fn)(void));

/* =============== Test definition helpers =============== */

/* Auto-register each test with GCC/Clang constructor.
 * If you need MSVC or other compilers, we can switch to manual registration.
 */
#ifdef __clang__
#define TEST_CASE(name)                                        \
    static void name(void);                                    \
    static void __attribute__((constructor)) _reg_##name(void) \
    {                                                          \
        test_framework_register(#name, name);                  \
    }                                                          \
    static void name(void)
#else
/* Fallback: user must call test_framework_register manually */
#define TEST_CASE(name) static void name(void)
#endif

#define TESTF(func_name)                                      \
    do {                                                      \
        test_framework_setup();                               \
        func_name();                                          \
        test_framework_teardown();                            \
    } while (0)

#define TESTB(block)                                          \
    do {                                                      \
        test_framework_setup();                               \
        do block while (0);                                   \
        test_framework_teardown();                            \
    } while (0)

#endif /* TEST_FRAMEWORK_H */
