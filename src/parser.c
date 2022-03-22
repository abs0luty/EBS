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

#include "parser.h"

static void advance(struct parser_state *state);

static void require_token(struct parser_state *state, int type);

static void print_error(struct parser_state *state, const char *message);

static struct component_AST *
parse_component_statement(struct parser_state *state);

static char **parse_sources(struct parser_state *state, size_t *sources_count,
                            struct code_location **startls,
                            struct code_location **endls);

static struct link_AST *parse_link_statement(struct parser_state *state);

static struct test_AST *parse_test_statement(struct parser_state *state);

struct parser_state *new_parser_state(struct lexer_state *lexer_state) {
  struct parser_state *parser_state = malloc(sizeof(struct parser_state));
  parser_state->lexer_state = lexer_state;
  parser_state->current_token = next_token(lexer_state);
  return parser_state;
}

void free_parser_state(struct parser_state *parser_state) {
  free_lexer_state(parser_state->lexer_state);
  free_token(parser_state->current_token);
  free(parser_state);
}

static void advance(struct parser_state *state) {
  state->current_token = next_token(state->lexer_state);

  if (state->current_token->type == ERROR_TOK) {
    print_error(state, format("unexcepted character: '%c' (+%d)",
                              state->current_token->value.chr,
                              (int)state->current_token->value.chr));
    advance(state);
  } else if (state->current_token->type == STRING_NOT_CLOSED_TOK) {
    print_error(state, "not closed string");
    advance(state);
  }
}

static void require_token(struct parser_state *state, int type) {
  if (state->current_token->type != type) {
    print_error(state, format("expected token of type %s, got %s",
                              dump_token_type(type),
                              dump_token_type(state->current_token->type)));
    exit(1);
  }
}

static void print_error(struct parser_state *state, const char *message) {
  println("[\"%s\" %ld:%ld-%ld:%ld]: %s", state->lexer_state->filename,
          state->current_token->startl->line,
          state->current_token->startl->column,
          state->current_token->endl->line, state->current_token->endl->column,
          message);
}

struct build_file_AST *parse_build_file(struct parser_state *state) {
  register size_t STATEMENTS_BUFFER_SIZE = 10000;

  register size_t statements_amount = 0;

  struct AST **statements =
      malloc(sizeof(struct AST *) * STATEMENTS_BUFFER_SIZE);

  while (state->current_token->type != EOF_TOK) {
    if (statements_amount == STATEMENTS_BUFFER_SIZE) {
      statements = realloc(statements, sizeof(struct AST *) *
                                           (STATEMENTS_BUFFER_SIZE *= 2));
    }

    statements[statements_amount++] = parse_statement(state);
  }

  return new_build_file_AST(statements, statements_amount);
}

struct AST *parse_statement(struct parser_state *state) {
  switch (state->current_token->type) {
  case COMPONENT_TOK:
    return AST_from_component_AST(parse_component_statement(state));
  case LINK_TOK:
    return AST_from_link_AST(parse_link_statement(state));
  case TEST_TOK:
    return AST_from_test_AST(parse_test_statement(state));
  default:
    print_error(state, format("unexpected token %s",
                              dump_token_type(state->current_token->type)));
    exit(1);
  }
}

static struct component_AST *
parse_component_statement(struct parser_state *state) {
  advance(state); // 'component'

  int component_type;

  switch (state->current_token->type) {
  case STATIC_TOK:
    component_type = STATICCK;
    break;
  case EXECUTABLE_TOK:
    component_type = EXECUTABLECK;
    break;
  default:
    print_error(state, format("unexpected token %s",
                              dump_token_type(state->current_token->type)));
    exit(1);
  }

  advance(state); // 'static' or 'executable'

  require_token(state, ID_TOK);
  char *component_name;
  component_name = state->current_token->value.str;

  advance(state); // component name

  require_token(state, COLON_TOK); // ';'
  advance(state);

  size_t sources_amount = 0;

  struct code_location **startls =
      malloc(sizeof(struct code_location *) * 10000);
  struct code_location **endls = malloc(sizeof(struct code_location *) * 10000);

  char **sources = parse_sources(state, &sources_amount, startls, endls);

  return new_component_AST(component_type, component_name, sources, startls,
                           endls, sources_amount);
}

static char **parse_sources(struct parser_state *state, size_t *sources_count,
                            struct code_location **startls,
                            struct code_location **endls) {
  register size_t count = 0;

  register size_t SOURCES_BUFFER_SIZE = 1000;

  char **sources = malloc(sizeof(char *) * SOURCES_BUFFER_SIZE);

  while (state->current_token->type != SEMICOLON_TOK) {
    if (count == SOURCES_BUFFER_SIZE) {
      sources = realloc(sources, sizeof(char *) * (SOURCES_BUFFER_SIZE *= 2));
    }

    require_token(state, STRING_TOK);

    startls[count] = copy_code_location(state->current_token->startl);
    sources[count] = state->current_token->value.str;
    endls[count] = copy_code_location(state->current_token->endl);
    count++;

    advance(state); // source name
  }

  *sources_count = count;

  advance(state); // ';'

  return sources;
}

static struct link_AST *parse_link_statement(struct parser_state *state) {
  advance(state); // 'link'

  require_token(state, ID_TOK);

  struct code_location *startl =
      copy_code_location(state->current_token->startl);
  char *main_component = state->current_token->value.str;
  struct code_location *endl = copy_code_location(state->current_token->endl);

  advance(state); // main component

  register size_t LINK_COMPONENTS_BUFFER_SIZE = 1000;
  register size_t count = 0;
  char **link_components = malloc(sizeof(char *) * LINK_COMPONENTS_BUFFER_SIZE);
  struct code_location **link_components_startls =
      malloc(sizeof(struct code_location *) * LINK_COMPONENTS_BUFFER_SIZE);
  struct code_location **link_components_endls =
      malloc(sizeof(struct code_location *) * LINK_COMPONENTS_BUFFER_SIZE);

  while (state->current_token->type != SEMICOLON_TOK) {
    if (LINK_COMPONENTS_BUFFER_SIZE == count) {
      link_components = realloc(
          link_components, sizeof(char *) * (LINK_COMPONENTS_BUFFER_SIZE *= 2));
    }

    require_token(state, ID_TOK);

    link_components[count] = state->current_token->value.str;
    link_components_startls[count] =
        copy_code_location(state->current_token->startl);
    link_components_endls[count] =
        copy_code_location(state->current_token->endl);

    count++;

    advance(state); // link component
  }

  advance(state); // ';'

  return new_link_AST(startl, endl, main_component, link_components,
                      link_components_startls, link_components_endls, count);
}

static struct test_AST *parse_test_statement(struct parser_state *state) {
  advance(state); // 'test'

  require_token(state, ID_TOK);

  struct code_location *startl =
      copy_code_location(state->current_token->startl);

  char *component = state->current_token->value.str;

  struct code_location *endl = copy_code_location(state->current_token->endl);

  advance(state); // component

  require_token(state, SEMICOLON_TOK); // ';'
  advance(state);

  return new_test_AST(startl, endl, component);
}