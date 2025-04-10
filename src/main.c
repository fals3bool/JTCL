#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "fs.h"
#include "tcat.h"

static char *CATALINA_HOME;
static char *WARFILE;

int readenv();
int runargs(char **argv);

const char *commands[3] = {"set", "run", "stop"};

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("WAR=\"file_name\" jcatl [%s|%s|%s]\n\n", commands[0], commands[1], commands[2]);
        return -1;
    }

    int renv = readenv();
    if (renv != 0)
        return renv;

    int result = runargs(argv);

    return result;
}

int readenv()
{
    CATALINA_HOME = getenv("CATALINA_HOME");
    WARFILE = getenv("WAR");

    if (!CATALINA_HOME)
    {
        printf("CATALINA_HOME no está en el path\n\n");
        return -1;
    }
    if (!WARFILE)
    {
        printf("WARFILE no definido: escriba 'WAR=\"file_name\" jcatl [option]\n\n");
        return -1;
    }

    return 0;
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
    else
    {
        printf("No se reconoce el comando\n\n");
        result = 1;
    }
}