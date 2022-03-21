#include "str.h"

static char* format_v(const char* format, va_list args);

char* format(const char* format, ...) {
    va_list args;
    va_start(args, format);
    char* str = format_v(format, args);
    va_end(args);
    return str;
}

int equals(const char* left, const char* right) {
    return strcmp(left, right) == 0;
}

static char* format_v(const char* format, va_list args) {
    va_list args_copy;
    va_copy(args_copy, args);
    size_t len = vsnprintf(NULL, 0, format, args_copy) + 1;
    char* out = malloc(len * sizeof(char));
    vsnprintf(out, len, format, args);
    va_end(args_copy);
    return out;
}