#ifndef _string_h_
#define _string_h_

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *format(const char *format, ...) __attribute__((format(printf, 1, 2)));

int equals(const char *left, const char *right);

#endif /* _string_h_ */