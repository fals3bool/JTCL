#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fs.h>

static char *CATALINA_HOME;
static char *WARFILE;

void readenv();
int runargs(char **argv);

const char *commands[3] = {"clean", "build", "run"};

int streq(const char *s1, const char *s2) { return strcmp(s1, s2) == 0; }

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("jtcl [%s|%s|%s]\n\n", commands[0], commands[1], commands[2]);
    return -1;
  }
  readenv();

  int result = runargs(argv);
  if (result != 0)
    perror("jtcl");
  return result;
}

void readenv() {
  CATALINA_HOME = getenv("CATALINA_HOME");
  WARFILE = findwar("target/");

  if (!CATALINA_HOME) {
    printf("CATALINA_HOME no está en el path\n\n");
    exit(-1);
  }
  if (!WARFILE) {
    printf("No se ha encontrado el archivo {.war}. Ejecute jtcl en la carpeta "
           "del proyecto java (carpeta padre de /target/).");
    exit(-1);
  }
}

int runargs(char **argv) {
  int result = 0;

  if (streq(argv[1], commands[0])) {
    result = cleanwar(WARFILE, CATALINA_HOME);
  } else if (streq(argv[1], commands[1])) {
    result = system("mvn clean package");
    if (result == 0)
      result += cpywar(WARFILE, CATALINA_HOME);
  } else if (streq(argv[1], commands[2])) {
    result = system("$CATALINA_HOME/bin/catalina.sh run");
  }

  return result;
}
