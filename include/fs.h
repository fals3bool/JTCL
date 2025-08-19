#ifndef JTCL_FS
#define JTCL_FS

/**
 * Finds a war file inside the current project folder
 */
char *findwar(char *targetfolder);

/**
 * Copies the WARFILE into $CATALINA_HOME/webapps/
 */
int cpywar(const char *WARFILE, const char *CATALINA_HOME);

/**
 * Removes the WARFILE & its folder from $CATALINA_HOME/webapps/
 */
int cleanwar(const char *WARFILE, const char *CATALINA_HOME);

/**
 * Removes the folder with the same name as WARFILE.war
 */
int rmwarfolder(const char *WARFILE, const char *CATALINA_HOME);

#endif
