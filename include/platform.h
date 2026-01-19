#ifndef JTCL_PLATFORM
#define JTCL_PLATFORM

#ifdef _WIN32
#include <direct.h>
#include <windows.h>
#define PATH_SEPARATOR '\\'
#define PATH_SEPARATOR_STR "\\"
#else
#include <sys/stat.h>
#include <unistd.h>
#define PATH_SEPARATOR '/'
#define PATH_SEPARATOR_STR "/"
#endif

/**
 * Platform-independent directory removal
 * Removes a directory and all its contents recursively
 */
int remove_directory(const char *path);

/**
 * Platform-independent path join
 * Combines path components with appropriate separator
 */
char *join_path(const char *base, const char *relative);

/**
 * Platform-independent check if file exists
 */
int file_exists(const char *path);

/**
 * Platform-independent check if path is a directory
 */
int is_directory(const char *path);

/**
 * Get the appropriate Tomcat startup command for the platform
 */
char *get_tomcat_command(const char *catalina_home);

#endif
