#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "fs.h"
#include "tcat.h"
#include "nc.h"

static char *CATALINA_HOME;
static char *WARFILE;

void readenv();
int runargs(char **argv);

const char *commands[5] = {"set", "run", "stop", "remove", "listen"};

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("WAR=\"file_name\" jcatl [%s|%s|%s]\n\n", commands[0], commands[1], commands[2]);
        return -1;
    }
    readenv();

    int result = runargs(argv);
    if (result != 0)
        perror("tcatl");
    return result;
}

void readenv()
{
    CATALINA_HOME = getenv("CATALINA_HOME");
    WARFILE = getenv("WAR");

    if (!CATALINA_HOME)
    {
        printf("CATALINA_HOME no está en el path\n\n");
        exit(-1);
    }
    if (!WARFILE)
    {
        printf("WARFILE no definido: escriba 'WAR=\"file_name\" jcatl [option]\n\n");
        exit(-1);
    }
}

int runargs(char **argv)
{
    int result;

    if (strcmp(argv[1], commands[0]) == 0)
    {
        result = cpywar(WARFILE, CATALINA_HOME);
        if (result != 0)
        {
            perror("No se pudo copiar el archivo war");
            return result;
        }
        printf("Arhivo %s.war copiado con éxito\n\n", WARFILE);
    }
    else if (strcmp(argv[1], commands[2]) == 0)
    {
        printf("SHUTING DOWN...\n");
        result = runsh(shutdown, WARFILE, CATALINA_HOME);
        if (result != 0)
            return result;
    }
    else if (strcmp(argv[1], commands[1]) == 0)
    {
        printf("STARTING...\n");
        result = runsh(startup, WARFILE, CATALINA_HOME);
        if (result != 0)
            return result;
    }
    else if (strcmp(argv[1], commands[3]) == 0)
    {
        result = rmwar(WARFILE, CATALINA_HOME);
        if (result != 0)
        {
            perror("No se pudo eliminar el proyecto (folder + .war)");
            return result;
        }
        printf("Arhivo %s.war y carpeta eliminados\n\n", WARFILE);
    }
    else if (strcmp(argv[1], commands[4]) == 0)
    {
        nc_listen(WARFILE, CATALINA_HOME);
        result = 0;
    }
    else
    {
        printf("No se reconoce el comando\n\n");
        result = 1;
    }

    return result;
}