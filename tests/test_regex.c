#include "test_framework.h"
#include <pregex.h>
#include <string.h>

TEST_CASE(test_convert_regex_to_postfix){
    char postfix[256];
    regex_to_postfix("ab|c*", postfix);
    ASSERT_TRUE(strcmp(postfix, "ab.c*|") == 0);

    regex_to_postfix("abab|abbb", postfix);
    ASSERT_TRUE(strcmp(postfix, "ab.a.b.ab.b.b.|") == 0);
}

TEST_CASE(test_pregex_match){
    ASSERT_TRUE(pregex_match("a|b", "a") == 1);
    ASSERT_TRUE(pregex_match("a|b", "b") == 1);
    ASSERT_TRUE(pregex_match("a|b", "c") == 0);

    ASSERT_TRUE(pregex_match("ab*", "a") == 1);
    ASSERT_TRUE(pregex_match("ab*", "ab") == 1);
    ASSERT_TRUE(pregex_match("ab*", "abb") == 1);
    ASSERT_TRUE(pregex_match("ab*", "ac") == 0);

    //ASSERT_TRUE(pregex_match("a*b", "b") == 1);
    //ASSERT_TRUE(pregex_match("a*b", "ab") == 1);
    //ASSERT_TRUE(pregex_match("a*b", "aaab") == 1);
    //ASSERT_TRUE(pregex_match("a*b", "aabx") == 0);

    //ASSERT_TRUE(pregex_match("a|bc*", "a") == 1);
    //ASSERT_TRUE(pregex_match("a|bc*", "b") == 1);
    //ASSERT_TRUE(pregex_match("a|bc*", "bc") == 1);
    //ASSERT_TRUE(pregex_match("a|bc*", "bcc") == 1);
    //ASSERT_TRUE(pregex_match("a|bc*", "d") == 0);
}
