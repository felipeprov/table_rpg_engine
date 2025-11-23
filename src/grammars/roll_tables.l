%option noyywrap
%option nounput
%option noinput
%option nounistd

%{
#include <roll_tables_gen.hpp> // adjust to your Bison output name
#include <string>
#include <iostream>

#ifdef _WIN32
    #include <io.h>
    #define isatty  _isatty
    #define fileno _fileno   // also removes the deprecation noise
#endif

using std::string;

// Where we are in the document
enum LexState {
    LS_DEFAULT,
    LS_AFTER_HASH,   // after "# " (reading table name)
    LS_HEADER_COL1,  // header first column (ROLL_TYPE)
    LS_HEADER_COL2,  // header second column (TABLE_NAME)
    LS_BODY_COL1,    // row first column (ROLL)
    LS_BODY_COL2     // row second column (ROLL_VALUE)
};

static LexState lex_state = LS_DEFAULT;
static bool seen_header = false;

// Trim surrounding spaces
static string trim(const string& s) {
    std::printf("triming string %s\n", s.c_str());
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

static char* dupstr(const char* s) {
    std::printf("dup string %s\n", s);
    size_t len = std::strlen(s);
    char* p = (char*)std::malloc(len + 1);
    if (!p) return nullptr;
    std::memcpy(p, s, len + 1);
    return p;
}

static const char* tokname(int t) {
    switch (t) {
        case '#':        return "CHAR('#')";
        case '|':        return "CHAR('|')";
        case NAME:       return "NAME";
        case ROLL_TYPE:  return "ROLL_TYPE";
        case TABLE_NAME: return "TABLE_NAME";
        case ROLL:       return "ROLL";
        case ROLL_VALUE: return "ROLL_VALUE";
        case NEWLINE:    return "NEWLINE";
        default:         return "OTHER";
    }
}


static int debug_token(int t, const char* text) {
    std::printf("LEX: %-12s \"%s\"\n", tokname(t), text);
    return t;
}

%}

NEWLINE     \r?\n

%%

^[ \t]*{NEWLINE} {
    // This is a pure empty line → swallow it
    // DO NOT return NEWLINE
    // Just ignore it
    // Debug
    std::printf("LEX: Ignoring blank line (CRLF)\n");
}


^[ \t]*\|[ \t-]*\|[ \t-]*\|[ \t]*{NEWLINE} {
    lex_state = LS_DEFAULT;
    std::printf("Ignoring tokens with new line");
    /* no tokens emitted */
}

^[ \t]*#[ \t]* {
    std::printf("LEX RULE: heading line start, text=\"%s\"\n", yytext);
    lex_state = LS_AFTER_HASH;
    return debug_token('#', "#");                // matches the '#' literal in the grammar
}

^[ \t]*\| {
    if (!seen_header) {
        seen_header = true;
        lex_state = LS_HEADER_COL1;
    } else {
        lex_state = LS_BODY_COL1;
    }
    return debug_token('|', "|");                // leading pipe
}


\| {
    switch(lex_state) {
        case LS_HEADER_COL1 : 
            lex_state = LS_HEADER_COL2;
            break;
        
        case LS_HEADER_COL2:
            lex_state = LS_DEFAULT;
            break;

        case LS_BODY_COL1:
            lex_state = LS_BODY_COL2;
            break;

        case LS_BODY_COL2:
            lex_state = LS_DEFAULT;
            break;
        
        default:
            break;
    }

    return debug_token('|', "|");
}

[^|\r\n]+ {
    string t = trim(yytext);
    std::printf("processing text %s", t.c_str());
    if(t.empty()) {

    }
    else {
        char *copy = dupstr(t.c_str());
        if (!copy) {
            // out of memory
            return 0;
        }
        else {
            switch(lex_state) {
                case LS_AFTER_HASH:
                    yylval.str = copy;
                    return debug_token(NAME, t.c_str());
                case LS_HEADER_COL1:
                    yylval.str = copy;
                    return debug_token(ROLL_TYPE, t.c_str());

                case LS_HEADER_COL2:
                    yylval.str = copy;
                    return debug_token(TABLE_NAME, t.c_str());

                case LS_BODY_COL1:
                    yylval.str = copy;
                    return debug_token(ROLL, t.c_str());

                case LS_BODY_COL2:
                    yylval.str = copy;
                    return debug_token(ROLL_VALUE, t.c_str());
                
                default:
                    std::free(copy);
                    break;
            }
            
        }
    }
}

{NEWLINE} {
    std::printf("LEX RULE: new line start, yyleng=%d, bytes=", yyleng);
    for (int i = 0; i < yyleng; ++i) {
        unsigned char c = (unsigned char)yytext[i];
        std::printf(" 0x%02X('%c')",
                     c,
                     (c >= 32 && c < 127) ? c : '.');
    }
    std::printf("\n");

    lex_state = LS_DEFAULT;
    return debug_token(NEWLINE, "\\n");
}

[ \t]+ {
    std::printf("Ignoring token");
    //ignore
}

\r {
    std::printf("Ignoring token");
    //ignore
}

. { 
    std::printf("Ignoring token");
    /* ignore */ 
}

%%

