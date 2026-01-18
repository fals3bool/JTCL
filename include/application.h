#ifndef JTCL_APPLICATION
#define JTCL_APPLICATION

/**
 * Returns allocated string with full path to application file (.war),
 * or NULL if not found
 */
char *get_application_path(void);

/**
 * Deploys aplication to $CATALINA_HOME/webapps/
 */
int deploy_app(const char *path, const char *catalina_home);

/**
 * Removes the application and its folder from $CATALINA_HOME/webapps/
 */
int remove_app(const char *path, const char *catalina_home);

#endif
