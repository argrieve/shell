#ifndef TYPES_H
#define TYPES_H

// User command modifiers
typedef struct
{
  char *out_file; // File to redirect stdout to
  char bg; // Run the command in the background
} mods_t;

#endif
