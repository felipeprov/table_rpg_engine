%{
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <memory>

#include <roll_tables.hpp>
#define YYERROR_VERBOSE 1

extern int yylex();
void yyerror(const char* msg);

RollTable* g_roll_table = nullptr;
%}

%union {
    char* str;
}

%debug
%define parse.error verbose


%token <str> NAME 
       <str> TABLE_NAME 
       <str> ROLL_TYPE 
       <str> ROLL 
       <str> ROLL_VALUE
        
%token NEWLINE 

%%

input: 
    file
;

file: 
    '#' NAME NEWLINE header rows
    {
        std::printf ("\n");
        free($2);
    }
;

header: '|' ROLL_TYPE '|' TABLE_NAME '|' NEWLINE {
    std::printf("Parser: HEADER %s %s\n", $2, $4);
    g_roll_table = new RollTable();
    g_roll_table->title.assign($2);
    g_roll_table->dice.assign($4);
    free($2);
    free($4);
};

rows:
    /* empty */
    | rows row

row:
    '|' ROLL '|' ROLL_VALUE '|' NEWLINE {
        std::printf("Parser: ROLL %s %s\n", $2, $4);
        RollRow row;
        row.range.assign($2);
        row.result.assign($4);
        g_roll_table->rows.push_back(row);
        free($2);
        free($4);
};

%%

void yyerror(const char* msg) {
    std::fprintf(stderr, "Parse error: %s\n", msg);
}
