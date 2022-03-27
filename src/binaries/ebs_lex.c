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

#include "../lexer/lexer.h"
#include "../lexer/token.h"
#include "../util/file.h"
#include "../util/println.h"
#include <time.h>

int main(size_t argc, const char **argv) {
  const char *filename = argv[1];

  if (!filename) {
    println("usage: %s <filename>", argv[0]);
    return 1;
  }

  char *input = read_file(filename);

  if (!input) {
    println("could not read file %s", filename);
    return 1;
  }

  struct lexer_state *state = new_lexer_state(filename, input);
  struct token *token = NULL;

  clock_t start = clock();

  while (token == NULL || token->type != EOF_TOK) {
    token = next_token(state);
    println("%s", dump_token(token));
  }

  println("finished in %f s.", (double)(clock() - start) / CLOCKS_PER_SEC);

  free_lexer_state(state);
  free_token(token);
}