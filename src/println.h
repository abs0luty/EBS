#ifndef _println_h_
#define _println_h_

#include <stdarg.h>
#include <stdio.h>

void println(const char *format, ...) __attribute__((format(printf, 1, 2)));

#endif /* _println_h_ */