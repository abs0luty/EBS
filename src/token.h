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

#ifndef _token_h_
#define _token_h_

#include "code_location.h"
#include "str.h"

union token_value {
  char *str;
  char chr;
};

#define TVALUE(key, value)                                                     \
  (union token_value) { .key = value }

struct token {
  enum {
    STRING_TOK,
    ID_TOK,
    COMPONENT_TOK,
    STATIC_TOK,
    LINK_TOK,
    EXECUTABLE_TOK,
    TEST_TOK,
    SEMICOLON_TOK,
    COLON_TOK,
    EOF_TOK,
    ERROR_TOK,
    STRING_NOT_CLOSED_TOK,
  } type;
  union token_value value;
  const struct code_location *startl, *endl;
};

struct token *new_token(int type, union token_value value,
                        const struct code_location *startl,
                        const struct code_location *endl);

char *dump_token(const struct token *token);

char *dump_token_type(int type);

char *dump_token_value(union token_value value, int type);

void free_token(struct token *token);

#endif /* _token_h_ */
