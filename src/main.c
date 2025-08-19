#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fs.h>

static char *CATALINA_HOME;
static char *WARFILE;

int readenv(int verbose);
int runargs(char **argv, int len);

const char *commands[3] = {"clean", "build", "run"};

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

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("jtcl [%s|%s|%s]\n\n", commands[0], commands[1], commands[2]);
    printf("jtcl help: for help :P\n\n");
    return -1;
  }
  if (strneq(argv[1], "help", "h", "-h", "--help", NULL)) {
    printf("JTCL - JTomcatLauncher [1.3.0]\n  Build & Deploy WAR files to Tomcat\n\n");
    printf("Uso:\n  $ jtcl <command>\n\n");
    printf("Comandos:\n");
    printf("  clean     Limpia /tomcat/webapps/. Elimina el .war y su carpeta.\n");
    printf("  build     Ejecuta 'mvn clean package' y copia el .war en /tomcat/webapps/.\n");
    printf("  run       Inicia Tomcat.\n\n");
    printf("Variables de Entorno:\n");
    printf("  $CATALINA_HOME     Ruta al directorio de Tomcat\n\n\n");
    printf("Ejemplo:\n\n  $ export CATALINA_HOME=/opt/apache-tomcat-11.0.5\n  $ jtcl clean build run\n\n");
    return 0;
  }

  int result = runargs(argv, argc);
  // if (result != 0)
  // perror("jtcl");
  return result;
}

int readenv(int verbose) {
  CATALINA_HOME = getenv("CATALINA_HOME");
  WARFILE = findwar("target/");

  if (!CATALINA_HOME) {
    if (verbose)
      printf("CATALINA_HOME no está en el path\n\n");
    return 1;
  }
  if (!WARFILE) {
    if (verbose)
      printf(
        "No se ha encontrado el archivo WAR. Ejecute jtcl en la carpeta "
        "del proyecto java (carpeta padre de /target/).\n\n");
    return 1;
  }

  return 0;
}

int runargs(char **argv, int len) {
  int result = 0;

  for (int i = 1; i < len; i++) {
    if (streq(argv[i], commands[0])) {
      if (readenv(0))
        continue;
      cleanwar(WARFILE, CATALINA_HOME);
      printf(" > CLEANED\n");
    } else if (streq(argv[i], commands[1])) {
      result += system("mvn clean package");
      result += readenv(1);
      if (result == 0) {
        result += cpywar(WARFILE, CATALINA_HOME);
      }
    } else if (streq(argv[i], commands[2])) {
      if (result == 0)
        result += system("$CATALINA_HOME/bin/catalina.sh run");
    } else {
      printf("Unknown command: {%s}\n\n", argv[i]);
      return -1;
    }
  }

  return result;
}
