#ifndef JTCL_STRING
#define JTCL_STRING

/**
 * Compares two strings and retruns true '1' if they are equals.
 * Returns false '0' otherwise.
 */
int streq(const char *s1, const char *s2);

/**
 * Compares a string with a list of strings.
 * Returns true '1' if it's equals to at least one of them, false '0' otherwise.
 */
int strneq(const char *s1, const char *ss, ...);

/**
 * Returns true '1' if the string contains the suffix, false '0' otherwise.
 */
int strendswith(const char *str, const char *suffix);

/**
 * Trims the string. Erases empty characters.
 */
char *strtrim(char *str);

#endif
