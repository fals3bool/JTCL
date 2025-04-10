#include "tcat.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char *server_state[2] = {"ENCENDIDO", "APAGADO"};

int runcmd(const char *sh, const char *CATALINA_HOME)
{
    size_t size = strlen(CATALINA_HOME) + strlen(sh);
    char *cmd = malloc(sizeof(char) * size);
    strcpy(cmd, "");
    strcat(cmd, CATALINA_HOME);
    strcat(cmd, sh);
    int result = system(cmd);
    free(cmd);
    return result;
}

int runsh(const enum sh shc, const char *WARFILE, const char *CATALINA_HOME)
{
    int result;
    int state;
    switch (shc)
    {
    case startup:
        result = runcmd("/bin/startup.sh", CATALINA_HOME);
        state = startup;
        break;
    case shutdown:
        result = runcmd("/bin/shutdown.sh", CATALINA_HOME);
        state = shutdown;
        break;
    default:
        break;
    }

    if (result == 0)
        printf("\nhttp://localhost:8080/%s [%s]\n\n", WARFILE, server_state[state]);
    else
        perror("tomcat");
    return result;
}