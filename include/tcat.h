

enum sh
{
    startup = 0,
    shutdown = 1
};

int runcmd(const char *sh, const char *CATALINA_HOME);

int runsh(const enum sh shc, const char *WARFILE, const char *CATALINA_HOME);