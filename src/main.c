#include "command.h"
#include "logger.h"

#include <stdio.h>

int runargs(char **argv, int len) {
  for (int i = 1; i < len; i++) {
    result_t result = run_command(argv[i]);
    if (result.code != OK) {
      error_handler(&result);
      return result.code;
    }
    LOG(OK, "%s", result.input);
  }
  return 0;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("use ´$ jtcl help´ for help :P\n\n");
    return -1;
  }

  if (!load_env())
    return 1;

  load_commands();
  return runargs(argv, argc);
}
