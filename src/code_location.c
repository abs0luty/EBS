/// MIT License
///
/// Copyright (c) 2022 Salimgereyev Adi
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in all
/// copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.

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
