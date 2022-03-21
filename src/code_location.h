#ifndef _code_location_h_
#define _code_location_h_

#include <stdlib.h>
#include "str.h"

struct code_location
{
	size_t index;

	size_t line, column;
};

struct code_location *new_code_location(size_t index,
					size_t line, size_t column);

struct code_location *copy_code_location(const struct code_location *location);

// can be used for tokenizing punctuators where we need to get
// a location of the next character
struct code_location *copy_advanced_code_location(const struct code_location *location);

char *dump_code_location(const struct code_location *location);

void free_code_location(struct code_location *location);

#endif /* _code_location_h_ */
