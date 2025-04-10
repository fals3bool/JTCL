#include "fs.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const char *webfolder = "/webapps/";
const char *targetfolder = "target/";
const char *fext = ".war";

int cpyFile(const char *src, const char *dest)
{
    FILE *filer = fopen(src, "rb");
    if (filer == NULL)
    {
        return -2;
    }

    FILE *filew = fopen(dest, "wb");
    if (filew == NULL)
    {
        fclose(filer);
        return -3;
    }

    char buffer[1024];
    size_t bytes;

    while ((bytes = fread(buffer, 1, sizeof(buffer), filer)) > 0)
    {
        fwrite(buffer, 1, bytes, filew);
    }

    fclose(filer);
    fclose(filew);

    return 0;
}

int cpywar(const char *WARFILE, const char *CATALINA_HOME)
{
    // char cwd[PATH_MAX]; // still dont know if it would be usefull on windows
    // if (!getcwd(cwd, sizeof(cwd)))
    // {
    //     perror("getcwd");
    //     return 1;
    // }

    size_t srcsize = strlen(targetfolder) + strlen(WARFILE) + strlen(fext);
    size_t destsize = strlen(CATALINA_HOME) + strlen(webfolder) + strlen(WARFILE) + strlen(fext);
    char *src = malloc(sizeof(char) * srcsize);
    strcpy(src, "");
    strcat(src, targetfolder);
    strcat(src, WARFILE);
    strcat(src, fext);
    char *dest = malloc(sizeof(char) * destsize);
    strcpy(dest, "");
    strcat(dest, CATALINA_HOME);
    strcat(dest, webfolder);
    strcat(dest, WARFILE);
    strcat(dest, fext);

    printf("SRC: %s\n", src);
    printf("DEST: %s\n", dest);

    int result = cpyFile(src, dest);

    free(src);
    free(dest);

    return result;
}

int rmwar(const char *WARFILE, const char *CATALINA_HOME)
{
    size_t size = strlen(CATALINA_HOME) + strlen(webfolder) + strlen(WARFILE) + strlen(fext);
    char *file = malloc(sizeof(char) * size);
    strcpy(file, "");
    strcat(file, CATALINA_HOME);
    strcat(file, webfolder);
    strcat(file, WARFILE);
    strcat(file, fext);
    int r = remove(file) + rmfolder(WARFILE, CATALINA_HOME);
    free(file);
    return r;
}

int rmfolder(const char *WARFILE, const char *CATALINA_HOME)
{
    const char *cmd_temp = "rm -rf ";
    size_t size = strlen(cmd_temp) + strlen(CATALINA_HOME) + strlen(webfolder) + strlen(WARFILE);
    char *cmd = (char *)malloc(size);
    strcpy(cmd, "");
    strcat(cmd, cmd_temp);
    strcat(cmd, CATALINA_HOME);
    strcat(cmd, webfolder);
    strcat(cmd, WARFILE);
    return system(cmd);
}
