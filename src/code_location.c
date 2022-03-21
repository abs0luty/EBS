#include "code_location.h"

struct code_location *new_code_location(size_t index,
					size_t line, size_t column)
{
	struct code_location *location = malloc(sizeof(struct code_location));
	location->index = index;
	location->line = line;
	location->column = column;
	return location;
}

struct code_location *copy_code_location(const struct code_location *location)
{
	return new_code_location(location->index, location->line,
				 location->column);
}

struct code_location *copy_advanced_code_location(const struct code_location *location)
{
	return new_code_location(location->index + 1, location->line,
				 location->column + 1);
}

char *dump_code_location(const struct code_location *location)
{
	return format("loc(%ld, %ld, %ld)", location->index, location->line,
		      location->column);
}

void free_code_location(struct code_location *location)
{
	free(location);
}
