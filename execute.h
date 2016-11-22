#ifndef EXECUTE_H
#define EXECUTE_H 

#include "types.h"
#include <unistd.h>

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
int execute_cmd(size_t argc, char *const argv[], const mods_t *const mods);

/*
 * Checks if the user entered a builtin command
 * 
 * Parameters:
 *   cmd - character array to check
 *
 * Returns:
 *   1 if cmd contains a builtin command, 0 otherwise
 */
char is_builtin(const char *const cmd);

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
int execute_builtin(size_t argc, char *const argv[], const mods_t *const mods);

#endif
