#include "command.h"

#include <stdio.h>

int runargs(char **argv, int len) {
  for (int i = 1; i < len; i++)
    if (run_command(argv[i]) != 0)
      return 1;
  return 0;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("jtcl help: for help :P\n\n");
    return -1;
  }

  // if (!load_env())
  //   return 1;

  load_commands();
  return runargs(argv, argc);
}
