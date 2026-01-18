#ifndef JTCL_COMMAND
#define JTCL_COMMAND

#define JTCL_VERSION "1.4.5"

/**
 * Loads jtcl commands
 */
void load_commands();

/**
 * Loads needed environmental variables
 */
int load_env();

/**
 * Excecutes a command by its name and returns the result
 */
int run_command(const char *name);

#endif
