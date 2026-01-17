#include "str.h"

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

int streq(const char *s1, const char *s2) { return strcmp(s1, s2) == 0; }

int strneq(const char *s1, const char *ss, ...) {
  va_list args;
  va_start(args, ss);

  if (streq(s1, ss))
    return 1;

  char *s_i;
  while ((s_i = va_arg(args, char *)) != NULL)
    if (streq(s1, s_i))
      return 1;

  va_end(args);
  return 0;
}

int strendswith(const char *str, const char *suffix) {
  if (!str || !suffix)
    return 0;
  size_t lenstr = strlen(str);
  size_t lensuffix = strlen(suffix);
  if (lensuffix > lenstr)
    return 0;
  return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

char *strtrim(char *str) {
  char *end;

  while (isspace((unsigned char)*str))
    str++;
  if (*str == 0)
    return str;

  end = str + strlen(str) - 1;
  while (end > str && isspace((unsigned char)*end))
    end--;

  end[1] = '\0';
  return str;
}
