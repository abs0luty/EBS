#include "file.h"

char *read_file(const char *filename)
{
  FILE *file = fopen(filename, "rb");

  if (!file)
    return NULL;

  fseek(file, 0, SEEK_END);
  size_t length = ftell(file);
  char *buffer = calloc(length, length);
  fseek(file, 0, SEEK_SET);

  if (buffer)
    fread(buffer, 1, length, file);
  fclose(file);
  return buffer;
}