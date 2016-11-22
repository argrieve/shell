#include "utils.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

/*
 * Checks for and prints any changes in the status of a process
 * 
 * Parameters:
 *   pid     - process ID to check
 *   options - options for waitpid()
 */
void check_child_status(int pid, int options)
{
  int ret = 0; 
  int status = 0;

  // Check status of the specified child
  ret = waitpid(pid, &status, options);

  // Print the change in status (if any)
  if (ret > 0) 
  {
    // Print child's PID
    printf("[%d] ", ret);

    // Print the exit status of the child
    if(WIFEXITED(status))
      printf("Exit %d", WEXITSTATUS(status));
    if(WIFSIGNALED(status))
      printf("Killed (%d)", WTERMSIG(status));
    printf("\n");
  }
}

/*
 * Redirects stdout to the specified file
 * 
 * Parameters:
 *   stream - file to redirect stdout to
 *
 * Returns:
 *   Saved file descriptor for stdout on success, -1 otherwise
 */
int redirect_stdout(const char *const stream)
{
  // Ignore NULL streams
  if (stream == NULL)
  return -1;

  int saved_fd;
  int new_fd;

  // Save current open file descriptor for stdout
  if ((saved_fd = dup(fileno(stdout))) == -1)
  {
    perror("ERROR: redirect_stdout(): dup():");
    return -1;
  }
	
  // Open a new file 
  if ((new_fd = open(stream, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)) == -1)
  {
    perror("ERROR: redirect_stdout(): open():");
    return -1;
  }

  // Redirect stdout to new file 
  fflush(stdout);
  if (dup2(new_fd, fileno(stdout)) == -1)
  {
    perror("ERROR: redirect_stdout(): dup2():");
    close(new_fd);
    return -1;
  }

  // Cleanup
  if (close(new_fd) == -1)
  {
    perror("ERROR: redirect_stdout(): close():");
    return -1;
  }

  return saved_fd;
}

/*
 * Restores stdout after a redirection
 * 
 * Parameters:
 *   fd - file descriptor for stdout
 *
 * Returns:
 *   0 on success, -1 otherwise
 */
int restore_stdout(int fd)
{
  // Restore stdout
  fflush(stdout);
  if (dup2(fd, fileno(stdout)) == -1)
  {
    perror("ERROR: restore_stdout(): dup2():");
    close(fd);
    return -1;
  }

  // Close old file descriptor
  if (close(fd) == -1)
  {
    perror("ERROR: restore_stdout(): close():");
    return -1;
  }

  return 0;
}

/*
 * Prints the help message
 */
void print_help()
{
  printf("Switches:\n"
         "  -h - prints this message\n"
         "  -p <val> - sets the prompt to <val>\n"
         "\n"
         "Built-in commands:\n"
         "  help - prints this message\n"
         "  exit - the shell terminates\n"
         "  pid  - the shell prints its process ID\n"
         "  ppid - the shell prints its parent's process ID\n"
         "  pwd  - print the current working directory\n"
         "  cd <dir>  - change the working directory to <dir>\n"
         "  get <var> - prints the value of the environment variable <var>\n"
         "  set <var> <value> - set environment variable <var> to <value>\n"
         "\n"
         "Modifiers (must be placed at end of command):\n"
         "  & - Execute the command in the background\n"
         "  > <file> - Redirect stdout to <file>\n"
        );
}
