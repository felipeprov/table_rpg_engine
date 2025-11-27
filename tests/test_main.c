#include "test_framework.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
    TestRegistry *r = test_framework_registry();

    /* --- List tests ------------------------------------------------- */
    if (argc == 2 && strcmp(argv[1], "--list") == 0) {
        for (size_t i = 0; i < r->count; ++i) {
            printf("%s\n", r->items[i].name);
        }
        return 0;
    }

    /* --- Run a single named test ----------------------------------- */
    if (argc == 3 && strcmp(argv[1], "--run") == 0) {
        const char *name = argv[2];
        for (size_t i = 0; i < r->count; ++i) {
            if (strcmp(r->items[i].name, name) == 0) {
                test_framework_failures = 0;
                r->items[i].fn();
                return (test_framework_failures == 0) ? 0 : 1;
            }
        }
        fprintf(stderr, "Unknown test: %s\n", name);
        return 1;
    }

    /* --- Default: run all tests ------------------------------------ */
    test_framework_failures = 0;
    for (size_t i = 0; i < r->count; ++i) {
        printf("[ RUN      ] %s\n", r->items[i].name);
        r->items[i].fn();
        if (test_framework_failures == 0)
            printf("[       OK ] %s\n", r->items[i].name);
        else
            printf("[  FAILED  ] %s\n", r->items[i].name);
    }

    if (test_framework_failures == 0) {
        printf("All tests passed!\n");
        return 0;
    } else {
        printf("%d assertion(s) failed.\n", test_framework_failures);
        return 1;
    }
}
