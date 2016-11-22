#include "execute.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>

/*
 * Executes a shell command entered by the user
 * 
 * Parameters:
 *   argc - number of arguments in the command
 *   argv - the user's command
 *   mods - special modifiers for the command
 *
 * Returns:
 *   0 on success, -1 otherwise
 */
int execute_cmd(size_t argc, char *const argv[], const mods_t *const mods)
{
  // Builtin command received
  if (is_builtin(argv[0]))
    return execute_builtin(argc, argv, mods);

  // Non builtin command received
  int ret = fork();
  if (ret == -1)
  {
    // fork() failed in the parent
    perror("ERROR: fork():");
  }
  else if (ret == 0)
  {
    // Child code
    printf("[%d] %s\n", getpid(), argv[0]);
    if (mods->out_file != NULL && redirect_stdout(mods->out_file) == -1)
      return -1;
    execvp(argv[0], argv);
    printf("ERROR: Cannot exec '%s'\n", argv[0]);
    return -1;
  } 
  else 
  {
    // Parent code
    if (mods->bg == 0)
      check_child_status(ret, 0);
  }

  return 0;
}

/*
 * Checks if the user entered a builtin command
 * 
 * Parameters:
 *   cmd - character array to check
 *
 * Returns:
 *   1 if cmd contains a builtin command, 0 otherwise
 */
char is_builtin(const char *const cmd)
{
  if (strcmp(cmd, "help") == 0 ||
      strcmp(cmd, "exit") == 0 ||
      strcmp(cmd, "pid")  == 0 ||
      strcmp(cmd, "ppid") == 0 ||
      strcmp(cmd, "pwd")  == 0 ||
      strcmp(cmd, "cd")   == 0 ||
      strcmp(cmd, "get")  == 0 ||
      strcmp(cmd, "set")  == 0)
  {
    return 1;
  }

  return 0;
}

/*
 * Executes a builtin shell command
 * 
 * Parameters:
 *   argc - number of arguments in the command
 *   argv - the user's command
 *   mods - special modifiers for the command
 *
 * Returns:
 *   0 on success, -1 otherwise
 */
int execute_builtin(size_t argc, char *const argv[], const mods_t *const mods)
{
  // Handle file redirection
  int fd;
  if (mods->out_file != NULL && (fd = redirect_stdout(mods->out_file)) == -1)
    return -1;

  // HELP
  if (strcmp(argv[0], "help") == 0)
  {
    print_help();
  }
  // EXIT
  else if (strcmp(argv[0], "exit") == 0)
  {
    return -1;
  }
  // PID
  else if (strcmp(argv[0], "pid") == 0)
  {
    printf("[%d] shell\n", getpid());	
  }
  // PPID
  else if (strcmp(argv[0], "ppid") == 0)
  {
    printf("[%d] parent\n", getppid());
  } 
  // PWD
  else if (strcmp(argv[0], "pwd") == 0)
  {
    char dir[PATH_MAX];
    if (getcwd(dir, sizeof(dir)) == NULL)
      perror("ERROR: getcwd():");	
    else
      printf("%s\n", dir);
  }
  // CD
  else if (strcmp(argv[0], "cd") == 0)
  {
    int err = 0;
    if (argc > 1) 
      err = chdir(argv[1]); 
    else 
      err = chdir(getenv("HOME"));
    if (err != 0)
      perror("ERROR: chdir():");
  }
  // GET
  else if (strcmp(argv[0], "get") == 0)
  {
    if (argc > 1)
    {
      char *var = getenv(argv[1]);
      if (var == NULL) 
        printf("ERROR: Unknown variable '%s'.\n", argv[1]);
      else 
        printf("%s\n", var); 
    }
    else
      printf("Usage: get <var>.\n");
  }
  // SET
  else if (strcmp(argv[0], "set") == 0)
  {
    if (argc == 1)
      printf("Usage: set <var> <value>\n"); 
    else if (argc == 2 && unsetenv(argv[1]) == -1)
      perror("ERROR: unsetenv():");
    else if (setenv(argv[1], argv[2], 1) == -1)
      perror("ERROR: setenv():");
  }

  // Revert file redirection
  if (mods->out_file != NULL && restore_stdout(fd) == -1)
    return -1;

  return 0;
}
