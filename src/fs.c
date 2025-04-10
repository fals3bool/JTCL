#include "fs.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
    const char *targetfolder = "target/";
    const char *webfolder = "/webapps/";
    const char *fext = ".war";
    size_t srcsize = strlen(targetfolder) + strlen(WARFILE) + strlen(fext);
    size_t destsize = strlen(CATALINA_HOME) + strlen(webfolder) + strlen(WARFILE) + strlen(fext);
    char *src = malloc(sizeof(char) * srcsize);
    strcat(src, targetfolder);
    strcat(src, WARFILE);
    strcat(src, fext);
    char *dest = malloc(sizeof(char) * destsize);
    strcat(dest, CATALINA_HOME);
    strcat(dest, webfolder);
    strcat(dest, WARFILE);
    strcat(dest, fext);

    // printf("SRC: %s\n", src);
    // printf("DEST: %s\n", dest);

    int result = cpyFile(src, dest);

    free(src);
    free(dest);

    return result;
}