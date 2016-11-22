#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <wordexp.h>
#include "types.h"
#include "parse.h"
#include "execute.h"
#include "utils.h"

// Default prompt
const char * const default_prompt = "308sh> ";

// Program main 
int main(int argc, char *argv[])
{
  // Setup prompt
  const char *prompt = default_prompt;

  // Handle switches/options for the shell
  int opt;
  opterr = 0;
  while ((opt = getopt(argc, argv, "hp:")) != -1)
  {
    switch (opt)
    {
      case 'h':
        print_help();
        exit(EXIT_SUCCESS);
      case 'p':
        prompt = optarg;
        break;
      case '?':
        if (optopt == 'p')
          printf("Option -%c requires an argument.\n", optopt);
      else
        printf("ERROR: Invalid option '-%c'. \n", optopt);
      default:
        exit(EXIT_FAILURE);
    }
  }

  // Input buffer
  char *buff = NULL;
  size_t buff_len = 0;

  // Structs containing user's command information
  wordexp_t cmd;
  mods_t mods;

  // Main loop
  while (1)
  {
    // Check for changes in status of any background processes
    check_child_status(-1, WNOHANG);

    // Print the prompt
    printf("%s", prompt);

    // Get a line of user input
    if (getline(&buff, &buff_len, stdin) == -1)
    {
      perror("ERROR: getline():");
      break;
    }

    // Parse the input
    if (parse_cmd(buff, &cmd, &mods) != 0)
      continue;

    // Execute the user's command
    if (execute_cmd(cmd.we_wordc, cmd.we_wordv, &mods) == -1)
      break;
  }

  // Cleanup
  free(buff);	
  wordfree(&cmd);

  exit(EXIT_SUCCESS);
}

