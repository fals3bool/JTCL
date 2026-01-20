#include "application.h"
#include "exception.h"
#include "logger.h"
#include "platform.h"
#include "strings.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *webfolder = "webapps";

// Helper function to extract XML tag content (supports multiline)
char *extract_tag_content(FILE *pom, const char *tag) {
  char open_tag[256];
  char close_tag[256];
  snprintf(open_tag, sizeof(open_tag), "<%s>", tag);
  snprintf(close_tag, sizeof(close_tag), "</%s>", tag);

  rewind(pom);
  char line[1024];
  int in_tag = 0;
  char *content = NULL;
  size_t content_size = 0;

  while (fgets(line, sizeof(line), pom)) {
    if (strstr(line, open_tag)) {
      in_tag = 1;
      char *start = strstr(line, open_tag);
      start += strlen(open_tag);
      char *end = strstr(start, close_tag);

      if (end) {
        // Content is on the same line
        size_t content_len = end - start;
        content = malloc(content_len + 1);
        if (content) {
          strncpy(content, start, content_len);
          content[content_len] = '\0';
          content = strtrim(content);
        }
        break;
      } else {
        // Content spans multiple lines
        char *trimmed = strtrim(start);
        if (strlen(trimmed) > 0) {
          content = strdup(trimmed);
          content_size = strlen(content);
        }
      }
    } else if (in_tag && strstr(line, close_tag)) {
      break;
    } else if (in_tag) {
      char *trimmed = strtrim(line);
      if (strlen(trimmed) > 0) {
        if (content) {
          content = realloc(content, content_size + strlen(trimmed) + 1);
          strcat(content, trimmed);
          content_size += strlen(trimmed);
        } else {
          content = strdup(trimmed);
          content_size = strlen(content);
        }
      }
    }
  }

  return content;
}

char *extract_tag(char *file, char *tag) {
  FILE *pom = fopen(file, "r");
  if (!pom) {
    LOG(ERROR, "Could not open %s\n", file);
    return NULL;
  }
  char *content = extract_tag_content(pom, tag);
  fclose(pom);
  return content;
}

char *find_war_from_pom(char *artifact_id, char *version, char *packaging) {
  artifact_id = extract_tag("pom.xml", "artifactId");
  version = extract_tag("pom.xml", "version");

  if (!artifact_id || !version) {
    LOG(ERROR, "Could not find 'artifactId' or 'version' in pom.xml");
    return NULL;
  }

  packaging = extract_tag("pom.xml", "packaging");
  if (!packaging)
    packaging = strdup("war");

  if (!streq(packaging, "war")) {
    LOG(ERROR, "Project packaging is '%s', expected 'war'", packaging);
    return NULL;
  }

  size_t path_len = strlen("target/") + strlen(artifact_id) + strlen(version) +
                    strlen(".war") + 2;
  char *war_path = malloc(path_len);
  if (!war_path)
    return NULL;

  snprintf(war_path, path_len, "target/%s-%s.war", artifact_id, version);
  return war_path;
}

void mfree(char *ptr) {
  if (ptr)
    free(ptr);
}

char *get_application_path(void) {
  char *artifact_id = NULL;
  char *version = NULL;
  char *packaging = NULL;
  char *war_path = find_war_from_pom(artifact_id, version, packaging);

  mfree(artifact_id);
  mfree(version);
  mfree(packaging);
  return war_path;
}

int cpyfile(const char *src, const char *dest) {
  FILE *filer = fopen(src, "rb");
  if (filer == NULL) {
    return ERR_IO;
  }
  FILE *filew = fopen(dest, "wb");
  if (filew == NULL) {
    fclose(filer);
    return ERR_IO;
  }

  char buffer[1024];
  size_t bytes;
  while ((bytes = fread(buffer, 1, sizeof(buffer), filer)) > 0) {
    fwrite(buffer, 1, bytes, filew);
  }

  fclose(filer);
  fclose(filew);

  return OK;
}

const char *get_filename(const char *path) {
  const char *filename = strrchr(path, PATH_SEPARATOR);
  if (!filename) {
    filename = path;
  } else {
    filename++; // Skip the path separator
  }
  return filename;
}

int deploy_app(const char *path, const char *catalina_home) {
  const char *filename = get_filename(path);

  char *webapps_path = join_path(catalina_home, webfolder);
  if (!webapps_path)
    return ERR_ALLOC;

  char *dest = join_path(webapps_path, filename);
  free(webapps_path);
  if (!dest)
    return ERR_ALLOC;

  LOG(INFO, "trying to deploy in %s", dest);

  int result = cpyfile(path, dest);
  free(dest);
  return result;
}

int remove_app(const char *path, const char *catalina_home) {
  const char *filename = get_filename(path);

  char *webapps_path = join_path(catalina_home, webfolder);
  if (!webapps_path)
    return ERR_ALLOC;

  // remove file

  char *file_path = join_path(webapps_path, filename);
  if (!file_path) {
    free(webapps_path);
    return ERR_ALLOC;
  }

  int remove_result = remove(file_path);
  free(file_path);

  // remove folder

  size_t name_len = strlen(filename) - 4;
  char *appfolder = malloc(name_len + 1);
  if (!appfolder)
    return ERR_ALLOC;
  strncpy(appfolder, filename, name_len);
  appfolder[name_len] = '\0';

  char *folder_path = join_path(webapps_path, appfolder);
  free(webapps_path);
  free(appfolder);
  if (!folder_path)
    return ERR_ALLOC;

  int folder_result = remove_directory(folder_path);
  free(folder_path);

  return fmax(0, fmax(remove_result, folder_result));
}
