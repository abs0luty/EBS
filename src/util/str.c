/// MIT License
///
/// Copyright (c) 2022 Salimgereyev Adi
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"),
/// to deal in the Software without restriction, including without limitation
/// the rights to use, copy, modify, merge, publish, distribute, sublicense,
/// and/or sell copies of the Software, and to permit persons to whom the
/// Software is furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included
/// in all copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
/// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
/// DEALINGS IN THE SOFTWARE.

#include "str.h"

static char *format_v(const char *format, va_list args);

char *format(const char *format, ...) {
  va_list args;
  va_start(args, format);
  char *str = format_v(format, args);
  va_end(args);
  return str;
}

int equals(const char *left, const char *right) {
  return strcmp(left, right) == 0;
}

static char *format_v(const char *format, va_list args) {
  va_list args_copy;
  va_copy(args_copy, args);
  size_t len = vsnprintf(NULL, 0, format, args_copy) + 1;
  char *out = malloc(len * sizeof(char));
  vsnprintf(out, len, format, args);
  va_end(args_copy);
  return out;
}