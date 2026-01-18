#include "command.h"
#include "application.h"
#include "strings.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *catalina_home;
static char *application_path;

typedef struct {
  char *name;
  int (*callback)();
} command_t;

static command_t *commands;
static int command_count = 0;

int clean() {
  printf("CLEAN!\n");
  return 0;
}

int deploy() {
  printf("DEPLOY!\n");
  return 0;
}

int build() {
  printf("BUILD!\n");
  return 0;
}

int run() {
  printf("RUN!\n");
  return 0;
}

int help() {
  printf("JTCL - Java Tomcat Launcher [%s]\n"
         "Build & Deploy WAR files to Tomcat\n\n",
         JTCL_VERSION);
  printf("Usage:\n  $ jtcl <command>\n\n");
  printf("Commands:\n");
  printf("  clean     Clean tomcat/webapps/. Remove .war file and its "
         "folder.\n");
  printf("  build     Execute 'mvn clean package' and deploy application "
         "(.war) to tomcat/webapps/.\n");
  printf("  deploy    Deploy application (.war) to tomcat/webapps/ without "
         "building it again.\n");
  printf("  run       Start Tomcat Server.\n\n");
  printf("Environmental Variables:\n");
  printf("  $CATALINA_HOME     Path to Tomcat directory\n\n\n");
  printf("Example:\n\n"
         "  $ export CATALINA_HOME=/opt/apache-tomcat\n"
         "  $ jtcl clean build run\n\n");
  return 0;
}

void add_command(command_t *cmd) {
  int i = command_count++;
  size_t size = sizeof(command_t);
  if (commands)
    commands = realloc(commands, size * command_count);
  else
    commands = malloc(size);
  void *dest = commands + i;
  memcpy(dest, cmd, size);
}

#define ADD_CMD(cmd) add_command(&(command_t){#cmd, cmd})

void load_commands() {
  ADD_CMD(clean);
  ADD_CMD(deploy);
  ADD_CMD(build);
  ADD_CMD(run);
  ADD_CMD(help);
}

int run_command(const char *name) {
  for (int i = 0; i < command_count; i++) {
    if (streq(commands[i].name, name))
      return commands[i].callback();
  }
  return 1;
}

int load_env() {
  catalina_home = getenv("CATALINA_HOME");
  application_path = get_application_path();

  if (!catalina_home) {
    printf("Error: CATALINA_HOME is not set\n\n");
    return 0;
  }
  if (!application_path) {
    printf("Error: java application not found. Ensure pom.xml exists "
           "and project is configured with packaging 'war'.\n\n");
    return 0;
  }

  return 1;
}
