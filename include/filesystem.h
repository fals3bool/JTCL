#ifndef JTCL_FILE_SYSTEM
#define JTCL_FILE_SYSTEM

/**
 * Finds war file location and name by parsing pom.xml
 * Returns allocated string with full path to WAR file, or NULL if not found
 */
char *findwar(void);

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
