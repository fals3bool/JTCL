#include "filesystem.h"
#include "platform.h"
#include "strings.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define JTCL_VERSION "1.4.4"

static char *CATALINA_HOME;
static char *WARFILE;

int readenv(int verbose);
int runargs(char **argv, int len);

const char *commands[4] = {"clean", "build", "deploy", "run"};

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("jtcl [%s|%s|%s|%s]\n\n", commands[0], commands[1], commands[2],
           commands[3]);
    printf("jtcl help: for help :P\n\n");
    return -1;
  }
  if (strneq(argv[1], "help", "h", "-h", "--help", NULL)) {
    printf("JTCL - Java Tomcat Launcher [%s]\n  Build & Deploy WAR files to "
           "Tomcat\n\n",
           JTCL_VERSION);
    printf("Usage:\n  $ jtcl <command>\n\n");
    printf("Commands:\n");
    printf("  clean     Clean tomcat/webapps/. Remove .war file and its "
           "folder.\n");
    printf("  build     Execute 'mvn clean package' and deploy .war to "
           "tomcat/webapps/.\n");
    printf("  deploy    Deploy .war to tomcat/webapps/ without "
           "building it again.\n");
    printf("  run       Start Tomcat Server.\n\n");
    printf("Environmental Variables:\n");
    printf("  $CATALINA_HOME     Path to Tomcat directory\n\n\n");
    printf("Example:\n\n  $ export CATALINA_HOME=/opt/apache-tomcat\n  "
           "$ jtcl clean build run\n\n");
    return 0;
  }

  int result = runargs(argv, argc);
  // if (result != 0)
  // perror("jtcl");
  return result;
}

int readenv(int verbose) {
  CATALINA_HOME = getenv("CATALINA_HOME");
  WARFILE = findwar();

  if (!CATALINA_HOME) {
    if (verbose)
      printf("CATALINA_HOME is not set\n\n");
    return 0;
  }
  if (!WARFILE) {
    if (verbose)
      printf("WAR file not found. Ensure pom.xml exists "
             "and project is configured with packaging 'war'.\n\n");
    return 0;
  }

  return 1;
}

int runargs(char **argv, int len) {
  int errors = 0;

  for (int i = 1; i < len; i++) {
    if (streq(argv[i], commands[0])) {
      if (!readenv(0))
        continue;
      cleanwar(WARFILE, CATALINA_HOME);
      printf(" > CLEANED\n");
    } else if (streq(argv[i], commands[1])) {
      int maven_error = system("mvn clean package");
      int env_success = readenv(1);
      // Attempt to copy regardless of Maven result
      if (env_success)
        errors += cpywar(WARFILE, CATALINA_HOME);
      else
        errors++;

      if (maven_error && env_success) {
        printf("Warning: Maven build failed! Old version deployed\n");
      } else if (!env_success) {
        printf("Error: Couldn't deploy application");
      }
    } else if (streq(argv[i], commands[2])) {
      if (readenv(1))
        errors += cpywar(WARFILE, CATALINA_HOME);
      else
        printf("Error: Couldn't deploy application\n");
    } else if (streq(argv[i], commands[3])) {
      if (!readenv(1))
        continue;
      if (!errors) {
        char *tomcat_cmd = get_tomcat_command(CATALINA_HOME);
        if (tomcat_cmd) {
          errors += system(tomcat_cmd);
          free(tomcat_cmd);
        } else {
          printf("Error: Tomcat startup script not found\n");
          errors++;
        }
      }
    } else {
      printf("Unknown command: {%s}\n\n", argv[i]);
      return -1;
    }
  }

  return errors;
}
