#include "parse.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>

/*
 * Removes leading and trailing whitespace from a character array
 * 
 * Parameters:
 *   str - character array to trim whitespace from
 *
 * Returns:
 *   Pointer to the trimmed character array
 */
char *trim_whitespace(char *str)
{
  char *end;

  // Trim leading whitespace
  while (isspace((unsigned char)*str)) str++;

  // All spaces
  if (*str == '\0')
    return str;

  // Trim trailing whitespace
  end = str + strlen(str) - 1;
  while (end > str && isspace((unsigned char)*end)) end--;

  // New null character
  *(end+1) = '\0';

  return str;
}

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
int parse_cmd(char *const buff, wordexp_t *const cmd, mods_t *const mods)
{
  // Clear modifiers struct
  mods->out_file = NULL;
  mods->bg = 0;

  // Trim whitespace so wordexp() doesn't complain
  char *trim = trim_whitespace(buff);

  char *ptr = NULL;

  // Run in background?
  if ((ptr = strstr(&(trim[strlen(trim)-1]), "&")) != NULL)
  {
    // User wants the command to run in the background, set bg and remove &
    mods->bg = 1;
    *ptr = ' ';
    trim = trim_whitespace(trim);
  }

  // Redirect output to file?
  if ((ptr = strstr(trim, ">")) != NULL)
  {
    // Locate the filename
    *ptr++ = '\0';
    while (isspace((unsigned char)*ptr)) ptr++;
    if (*ptr != '\0')
      mods->out_file = ptr;
  }

  // Error check trimmed string before calling wordexp()
  if (trim == NULL || strlen(trim) == 0) 
    return -1;

  // Expand the user input string into a 2D array of one word strings
  if (wordexp(trim, cmd, 0) != 0)
  {
    printf("ERROR: Syntax error near unexpected token '%s'\n", buff);
    return -1;
  }

  return 0;
}

