#ifndef PARSE_H
#define PARSE_H

#include "types.h"
#include <wordexp.h>

/*
 * Removes leading and trailing whitespace from a character array
 * 
 * Parameters:
 *   str - character array to trim whitespace from
 *
 * Returns:
 *   Pointer to the trimmed character array
 */
char *trim_whitespace(char *str);

/*
 * Parses a command from user
 * 
 * Parameters:
 *   buff - input buffer containing the user's command
 *   cmd  - destination for shell-like expansion of the input command
 *   mods - special modifiers entered by the user
 *
 * Returns:
 *   0 on success, -1 otherwise
 */
int parse_cmd(char *const buff, wordexp_t *const cmd, mods_t *const mods);

#endif
