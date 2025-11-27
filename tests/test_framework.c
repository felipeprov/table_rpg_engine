#include "test_framework.h"

#include <stdio.h>
#include <stdlib.h>

int test_framework_failures = 0;

/* Default empty setup/teardown, so TESTF/TESTB work
 * even if the user never defines SETUP/TEARDOWN.
 * If you define SETUP/TEARDOWN in some test file, those
 * definitions will override these weak ones.
 */
__attribute__((weak)) void test_framework_setup(void) {}
__attribute__((weak)) void test_framework_teardown(void) {}

void test_framework_assert_true(bool condition,
                                const char *expr,
                                const char *file,
                                int line)
{
    if (!condition) {
        fprintf(stderr, "Assertion failed: %s at %s:%d\n",
                expr, file, line);
        ++test_framework_failures;
    }
}

/* Registry implementation */

static TestRegistry global_registry = { NULL, 0u, 0u };

TestRegistry *test_framework_registry(void)
{
    return &global_registry;
}

void test_framework_register(const char *name, void (*fn)(void))
{
    TestRegistry *r = &global_registry;

    if (r->count == r->capacity) {
        size_t new_cap = (r->capacity == 0u) ? 16u : (r->capacity * 2u);
        TestCase *new_items =
            (TestCase *)realloc(r->items, new_cap * sizeof(TestCase));
        if (!new_items) {
            fprintf(stderr, "Out of memory while registering test '%s'\n", name);
            exit(EXIT_FAILURE);
        }
        r->items = new_items;
        r->capacity = new_cap;
    }

    r->items[r->count].name = name;
    r->items[r->count].fn   = fn;
    r->count++;
}
