#pragma once

/**
 * @brief checks if a regex matches the pattern.
 * It assumes NULL finished strings.
 *
 * @param pattern regex pattern to be match
 * @param string string to be tests
 * @return 1 it matches, 0 do not
 */
int pregex_match(const char *pattern, const char *string);
void regex_to_postfix(const char *regex, char *postfix);
