#ifndef UTILS_H
#define UTILS_H

/*
 * Checks for and prints any changes in the status of a process
 * 
 * Parameters:
 *   pid     - process ID to check
 *   options - options for waitpid()
 */
void check_child_status(int pid, int options);

/*
 * Redirects stdout to the specified file
 * 
 * Parameters:
 *   stream - file to redirect stdout to
 *
 * Returns:
 *   Saved file descriptor for stdout on success, -1 otherwise
 */
int redirect_stdout(const char *const stream);

/*
 * Restores stdout after a redirection
 * 
 * Parameters:
 *   fd - file descriptor for stdout
 *
 * Returns:
 *   0 on success, -1 otherwise
 */
int restore_stdout(int fd);

/*
 * Prints the help message
 */
void print_help();

#endif
