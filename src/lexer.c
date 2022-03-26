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

#include "lexer.h"

#define current_char(state) (state->input[state->location->index])
#define lexing_finished(state) (current_char(state) == '\0')

static void skip_whitespace(struct lexer_state *state);

static inline char lookahead(const struct lexer_state *state);

static void advance(struct lexer_state *state);

static struct token *next_string_token(struct lexer_state *state);

// if chr is letter or under score (1-st element of identifier)
static int alpha1(char chr);

// if chr is letter, digit or under score (next element of identifier)
static int alpha2(char chr);

static struct token *next_name_token(struct lexer_state *state);

#define check_keyword(name, keyword, tok_type)                                 \
  if (equals(name, keyword)) {                                                 \
    return new_token(tok_type, TVALUE(str, name), startl, endl);               \
  }

struct lexer_state *new_lexer_state(const char *filename, const char *input) {
  struct lexer_state *state = malloc(sizeof(struct lexer_state));
  state->filename = filename;
  state->input = input;
  state->location = new_code_location(0, 1, 0);
  return state;
}

struct token *next_token(struct lexer_state *state) {
  if (isspace(current_char(state)))
    skip_whitespace(state);

  // skip comment
  if (current_char(state) == '/' && lookahead(state) == '/') {
    advance(state);
    advance(state);
    while (current_char(state) != '\n' && !lexing_finished(state))
      advance(state);
  }

  if (current_char(state) == '"')
    return next_string_token(state);

  if (current_char(state) == ';') {
    struct token *token = new_token(
        SEMICOLON_TOK, TVALUE(chr, ';'), copy_code_location(state->location),
        copy_advanced_code_location(state->location));
    advance(state);
    return token;
  }
  if (current_char(state) == ':') {
    struct token *token = new_token(
        COLON_TOK, TVALUE(chr, ':'), copy_code_location(state->location),
        copy_advanced_code_location(state->location));
    advance(state);
    return token;
  }

  if (alpha1(current_char(state)))
    return next_name_token(state);

  if (lexing_finished(state))
    return new_token(EOF_TOK, TVALUE(chr, (char)0), state->location,
                     copy_advanced_code_location(state->location));

  struct token *error_token =
      new_token(ERROR_TOK, TVALUE(chr, current_char(state)), state->location,
                copy_advanced_code_location(state->location));
  advance(state);
  return error_token;
}

void free_lexer_state(struct lexer_state *state) {
  free_code_location(state->location);
  free(state);
}

static void skip_whitespace(struct lexer_state *state) {
  while (isspace(current_char(state)))
    advance(state);
}

static inline char lookahead(const struct lexer_state *state) {
  if (state->location->index + 1 >= strlen(state->input))
    return '\0';
  return state->input[state->location->index + 1];
}

static void advance(struct lexer_state *state) {
  if (current_char(state) == '\n') {
    state->location->line++;
    state->location->column = 0;
  } else {
    state->location->column++;
  }

  state->location->index++;
}

static struct token *next_string_token(struct lexer_state *state) {
  advance(state);

  char *string_buffer = malloc(1024 * sizeof(char));
  register size_t index = 0;
  struct code_location *startl = copy_code_location(state->location);

  while (current_char(state) != '"' && !lexing_finished(state)) {
    string_buffer[index++] = current_char(state);
    advance(state);
  }

  if (current_char(state) != '"') {
    free(string_buffer);
    return new_token(STRING_NOT_CLOSED_TOK, TVALUE(chr, (char)0), startl,
                     copy_code_location(state->location));
  }

  advance(state);

  struct code_location *endl = copy_code_location(state->location);

  string_buffer[index] = '\0';

  return new_token(STRING_TOK, TVALUE(str, string_buffer), startl, endl);
}

static int alpha1(char chr) { return isalpha(chr); }

static int alpha2(char chr) { return isalnum(chr); }

static struct token *next_name_token(struct lexer_state *state) {
  char *name_buffer = malloc(1024 * sizeof(char));
  register size_t index = 0;
  struct code_location *startl = copy_code_location(state->location);

  while (alpha2(current_char(state))) {
    name_buffer[index++] = current_char(state);
    advance(state);
  }

  name_buffer[index] = '\0';

  struct code_location *endl = copy_code_location(state->location);

  check_keyword(name_buffer, "component", COMPONENT_TOK);
  check_keyword(name_buffer, "executable", EXECUTABLE_TOK);
  check_keyword(name_buffer, "static", STATIC_TOK);
  check_keyword(name_buffer, "link", LINK_TOK);
  check_keyword(name_buffer, "test", TEST_TOK);

  return new_token(ID_TOK, TVALUE(str, name_buffer), startl, endl);
}