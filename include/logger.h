#ifndef JTCL_LOGGER
#define JTCL_LOGGER

#ifdef _WIN32
#include <windows.h>

static WORD win_color_from_level(const char *level) {
  if (!strcmp(level, "ERROR") || !strcmp(level, "SEVERE"))
    return FOREGROUND_RED | FOREGROUND_INTENSITY;
  if (!strcmp(level, "WARNING"))
    return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
  if (!strcmp(level, "INFO"))
    return FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
  return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
}

#define LOG(level, format, ...)                                                \
  do {                                                                         \
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);                                \
    CONSOLE_SCREEN_BUFFER_INFO info;                                           \
    GetConsoleScreenBufferInfo(h, &info);                                      \
    SetConsoleTextAttribute(h, win_color_from_level(#level));                  \
    printf("JTCL [%s] " format "\n", #level, ##__VA_ARGS__);                   \
    SetConsoleTextAttribute(h, info.wAttributes);                              \
  } while (0)
#else
#define COLOR_ERROR "\x1b[31m"
#define COLOR_SEVERE "\x1b[31m"
#define COLOR_WARNING "\x1b[33m"
#define COLOR_INFO "\x1b[36m"
#define COLOR_OK "\x1b[32m"
#define COLOR_RESET "\x1b[0m"

#define LOG(level, format, ...)                                                \
  do {                                                                         \
    printf("JTCL [" COLOR_##level "%s" COLOR_RESET "] " format "\n", #level,   \
           ##__VA_ARGS__);                                                     \
  } while (0)
#endif

#endif
