#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <stdexcept>
#include <string>

#include <roll_tables_gen.hpp>   // Bison-generated
#include "roll_tables.hpp"       // Your AST structs

extern FILE* yyin;
extern int yyparse();
extern RollTable* g_roll_table;

RollTable parse_roll_table_from_file_s(const std::string& path)
{
    FILE* f = std::fopen(path.c_str(), "rb");
    if (!f)
        throw std::runtime_error("Could not open file: " + path);

    yyin = f;              // tell Flex to read from file
    g_roll_table = nullptr;

    if (yyparse() != 0) {
        fclose(f);
        throw std::runtime_error("Failed to parse: " + path);
    }

    fclose(f);

    if (!g_roll_table)
        throw std::runtime_error("Parser returned null AST: " + path);

    RollTable result = *g_roll_table;
    delete g_roll_table;
    g_roll_table = nullptr;

    return result;
}

RollTable parse_roll_table_from_file(const std::string& path)
{
    FILE* f = std::fopen(path.c_str(), "rb");
    if (!f)
        throw std::runtime_error("Could not open file: " + path);

    // DEBUG: peek first byte of the file
    int c = std::fgetc(f);
    if (c == EOF) {
        std::fprintf(stderr, "DEBUG: file is empty\n");
    } else {
        std::fprintf(stderr, "DEBUG: first byte = 0x%02X '%c'\n",
                     (unsigned char)c,
                     (c >= 32 && c < 127) ? c : '.');
        std::ungetc(c, f); // put it back
    }

    yydebug = 1;

    yyin = f;              // tell Flex to read from file
    g_roll_table = nullptr;

    if (yyparse() != 0) {
        std::fclose(f);
        throw std::runtime_error("Failed to parse: " + path);
    }

    std::fclose(f);

    if (!g_roll_table)
        throw std::runtime_error("Parser returned null AST: " + path);

    RollTable result = *g_roll_table;
    delete g_roll_table;
    g_roll_table = nullptr;

    return result;
}