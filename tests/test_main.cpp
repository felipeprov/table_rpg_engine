// test_main.cpp
#include "test_framework.hpp"
#include <cstring>

int main(int argc, char** argv) {
    using namespace test_framework;

    // --- List tests -------------------------------------------------
    if (argc == 2 && std::strcmp(argv[1], "--list") == 0) {
        for (auto& t : registry()) {
            std::cout << t.name << '\n';
        }
        return 0;
    }

    // --- Run a single named test -----------------------------------
    if (argc == 3 && std::strcmp(argv[1], "--run") == 0) {
        const char* name = argv[2];
        for (auto& t : registry()) {
            if (t.name == name) {
                failures = 0;
                t.fn();
                return failures == 0 ? 0 : 1;
            }
        }
        std::cerr << "Unknown test: " << name << '\n';
        return 1;
    }

    // --- Default: run all tests ------------------------------------
    failures = 0;
    for (auto& t : registry()) {
        std::cout << "[ RUN      ] " << t.name << '\n';
        t.fn();
        if (failures == 0)
            std::cout << "[       OK ] " << t.name << '\n';
        else
            std::cout << "[  FAILED  ] " << t.name << '\n';
    }

    if (failures == 0) {
        std::cout << "All tests passed!\n";
        return 0;
    } else {
        std::cout << failures << " assertion(s) failed.\n";
        return 1;
    }
}
