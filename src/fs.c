#include "fs.h"

#include <dirent.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *webfolder = "/webapps/";
const char *targetfolder = "target/";
const char *fext = ".war";

int strendcmp(const char *str, const char *suffix) {
  if (!str || !suffix)
    return 0;
  size_t lenstr = strlen(str);
  size_t lensuffix = strlen(suffix);
  if (lensuffix > lenstr)
    return 0;
  return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

char *findwar(char *targetfolder) {
  DIR *dirp = opendir(targetfolder);
  if (!dirp)
    return NULL;
  struct dirent *dp;
  while ((dp = readdir(dirp)) != NULL) {
    if (strendcmp(dp->d_name, fext)) {
      closedir(dirp);
      return dp->d_name;
    }
  }
  closedir(dirp);
  return NULL;
}

int cpyfile(const char *src, const char *dest) {
  FILE *filer = fopen(src, "rb");
  if (filer == NULL) {
    return -2;
  }

  FILE *filew = fopen(dest, "wb");
  if (filew == NULL) {
    fclose(filer);
    return -3;
  }

  char buffer[1024];
  size_t bytes;

  while ((bytes = fread(buffer, 1, sizeof(buffer), filer)) > 0) {
    fwrite(buffer, 1, bytes, filew);
  }

  fclose(filer);
  fclose(filew);

  return 0;
}

int cpywar(const char *WARFILE, const char *CATALINA_HOME) {
  size_t srcsize = strlen(targetfolder) + strlen(WARFILE) + 1;
  size_t destsize = strlen(CATALINA_HOME) + strlen(webfolder) +
                    strlen(WARFILE) + strlen(fext);
  char src[srcsize];
  snprintf(src, srcsize, "%s%s", targetfolder, WARFILE);
  char dest[destsize];
  snprintf(dest, destsize, "%s%s%s", CATALINA_HOME, webfolder, WARFILE);

  printf("SRC: %s\n", src);
  printf("DEST: %s\n", dest);

  int result = cpyfile(src, dest);

  return result;
}

int cleanwar(const char *WARFILE, const char *CATALINA_HOME) {
  size_t size = strlen(CATALINA_HOME) + strlen(webfolder) + strlen(WARFILE) + 1;
  char file[size];
  snprintf(file, size, "%s%s%s", CATALINA_HOME, webfolder, WARFILE);
  int r = remove(file) + rmwarfolder(WARFILE, CATALINA_HOME);
  return r;
}

int rmwarfolder(const char *WARFILE, const char *CATALINA_HOME) {
  const char *cmd_tmpl = "rm -rf ";
  char warfolder[strlen(WARFILE) - 3];
  strncpy(warfolder, WARFILE, strlen(WARFILE) - 4);
  size_t size = strlen(cmd_tmpl) + strlen(CATALINA_HOME) + strlen(webfolder) +
                strlen(WARFILE);
  char cmd[size + 1];
  snprintf(cmd, size, "%s%s%s%s", cmd_tmpl, CATALINA_HOME, webfolder,
           warfolder);
  cmd[size] = '\0';
  int result = system(cmd);
  return result;
}
