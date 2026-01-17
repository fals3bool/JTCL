#include "fs.h"
#include "str.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define JTCL_VERSION "1.4.2"

static char *CATALINA_HOME;
static char *WARFILE;

int readenv(int verbose);
int runargs(char **argv, int len);

const char *commands[3] = {"clean", "build", "run"};

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("jtcl [%s|%s|%s]\n\n", commands[0], commands[1], commands[2]);
    printf("jtcl help: for help :P\n\n");
    return -1;
  }
  if (strneq(argv[1], "help", "h", "-h", "--help", NULL)) {
    printf("JTCL - Java Tomcat Launcher [%s]\n  Build & Deploy WAR files to "
           "Tomcat\n\n", JTCL_VERSION);
    printf("Usage:\n  $ jtcl <command>\n\n");
    printf("Comands:\n");
    printf(
        "  clean     Limpia /tomcat/webapps/. Elimina el .war y su carpeta.\n");
    printf("  build     Ejecuta 'mvn clean package' y copia el .war (desde "
           "pom.xml) en /tomcat/webapps/.\n");
    printf("  run       Inicia Tomcat.\n\n");
    printf("Environmental Variables:\n");
    printf("  $CATALINA_HOME     Ruta al directorio de Tomcat\n\n\n");
    printf("Ejemplo:\n\n  $ export CATALINA_HOME=/opt/apache-tomcat\n  "
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
      printf("CATALINA_HOME no está en el path\n\n");
    return 0;
  }
  if (!WARFILE) {
    if (verbose)
      printf(
          "No se ha encontrado el archivo WAR. Asegúrese de que pom.xml exista "
          "y que el proyecto esté configurado como packaging 'war'.\n\n");
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
        printf("Maven build failed! Old version deployed\n");
      }
    } else if (streq(argv[i], commands[2])) {
      if (!errors)
        errors += system("$CATALINA_HOME/bin/catalina.sh run");
    } else {
      printf("Unknown command: {%s}\n\n", argv[i]);
      return -1;
    }
  }

  return errors;
}
