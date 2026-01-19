#include "platform.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <io.h>
#define access _access
#define F_OK 0
#endif

int remove_directory(const char *path) {
#ifdef _WIN32
  char cmd[MAX_PATH * 2];
  snprintf(cmd, sizeof(cmd), "rmdir /s /q \"%s\"", path);
  return system(cmd);
#else
  char cmd[1024];
  snprintf(cmd, sizeof(cmd), "rm -rf \"%s\"", path);
  return system(cmd);
#endif
}

char *join_path(const char *base, const char *relative) {
  if (!base || !relative)
    return NULL;

  size_t base_len = strlen(base);
  size_t rel_len = strlen(relative);

  // Check if base already ends with path separator
  int needs_separator = (base_len > 0 && base[base_len - 1] != PATH_SEPARATOR);

  size_t total_len = base_len + rel_len + (needs_separator ? 1 : 0) + 1;
  char *result = malloc(total_len);

  if (!result)
    return NULL;

  strcpy(result, base);
  if (needs_separator) {
    result[base_len] = PATH_SEPARATOR;
    result[base_len + 1] = '\0';
    strcat(result, relative);
  } else {
    strcat(result, relative);
  }

  return result;
}

int file_exists(const char *path) { return access(path, F_OK) == 0; }

int is_directory(const char *path) {
#ifdef _WIN32
  DWORD attributes = GetFileAttributesA(path);
  return (attributes != INVALID_FILE_ATTRIBUTES) &&
         (attributes & FILE_ATTRIBUTE_DIRECTORY);
#else
  struct stat statbuf;
  if (stat(path, &statbuf) != 0)
    return 0;
  return S_ISDIR(statbuf.st_mode);
#endif
}

char *get_tomcat_command(const char *catalina_home) {
  if (!catalina_home)
    return NULL;

#ifdef _WIN32
  char *startup_bat = join_path(catalina_home, "bin\\startup.bat");
  if (file_exists(startup_bat)) {
    char *cmd = malloc(strlen(startup_bat) + 8);
    if (cmd) {
      sprintf(cmd, "\"%s\"", startup_bat);
    }
    free(startup_bat);
    return cmd;
  }
  free(startup_bat);
  return NULL;
#else
  char *catalina_sh = join_path(catalina_home, "/bin/catalina.sh");
  printf("catalina sh: %s\n", catalina_sh);
  if (file_exists(catalina_sh)) {
    char *cmd = malloc(strlen(catalina_sh) + 20);
    if (cmd) {
      sprintf(cmd, "\"%s\" run", catalina_sh);
    }
    free(catalina_sh);
    return cmd;
  }
  free(catalina_sh);
  return NULL;
#endif
}
