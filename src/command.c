#include "command.h"
#include "application.h"
#include "exception.h"
#include "platform.h"
#include "strings.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *catalina_home;
static char *application_path;

result_code_t clean() { return remove_app(application_path, catalina_home); }

result_code_t deploy() { return deploy_app(application_path, catalina_home); }

result_code_t build() {
  int build_result = system("mvn clean package");
  int deploy_result = deploy_app(application_path, catalina_home);
  if (build_result != 0 && deploy_result == 0)
    return ERR_BUILD_BUT_DEPLOYED;
  if (build_result)
    return ERR_BUILD;
  if (deploy_result)
    return ERR_DEPLOY;
  return OK;
}

result_code_t run() {
  char *tomcat_cmd = get_tomcat_command(catalina_home);
  if (!tomcat_cmd)
    return ERR_TOMCAT_STARTUP;
  int tomcat_result = system(tomcat_cmd);
  if (tomcat_result != 0)
    return ERR_TOMCAT_INTERRUPT;
  return OK;
}

result_code_t help() {
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
  return SILENCE;
}

static command_t *commands;
static int command_count = 0;

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

result_t run_command(const char *name) {
  for (int i = 0; i < command_count; i++) {
    if (streq(commands[i].name, name))
      return (result_t){commands[i].name, commands[i].callback()};
  }
  return (result_t){name, ERR_NOT_FOUND};
}

void error_handler(const result_t *result) {
  switch (result->code) {
  case ERR_NOT_FOUND:
    printf("ERROR: %s -> Unknown command\n Try ´$ jtcl help´\n", result->input);
    break;

  case ERR_BUILD:
    printf("ERROR: %s -> Could not build the application\n", result->input);
    break;

  case ERR_BUILD_BUT_DEPLOYED:
    printf("ERROR: %s -> Build failed!\n"
           "WARNING: Deployed old version\n",
           result->input);
    break;

  case ERR_DEPLOY:
    printf("ERROR: %s -> Could not deploy java application\n", result->input);
    break;

  case ERR_IO:
    printf("ERROR: %s -> File not found\n", result->input);
    break;

  case ERR_ALLOC:
    printf("ERROR: %s -> Execution time error: Allocation\n", result->input);
    break;

  case ERR_TOMCAT_STARTUP:
    printf("ERROR: %s -> Tomcat startup script not found\n", result->input);
    break;

  case ERR_TOMCAT_INTERRUPT:
    printf("ERROR: %s -> Tomcat interrupted unexpectedly\n", result->input);
    break;

  case SILENCE:
    // #Engineering #CleanCode #Art #Amazing
    break;

  default:
    printf("ERROR: An unexpected error ocurred\n");
    break;
  }
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
