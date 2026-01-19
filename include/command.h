#ifndef JTCL_COMMAND
#define JTCL_COMMAND

#include "exception.h"

#define JTCL_VERSION "1.4.5"

typedef struct {
  char *name;
  result_code_t (*callback)();
} command_t;

typedef struct {
  const char *input;
  result_code_t code;
} result_t;

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
result_t run_command(const char *name);

/**
 * Command error handler. Call it when result_code != OK.
 */
void error_handler(const result_t *result);

#endif
