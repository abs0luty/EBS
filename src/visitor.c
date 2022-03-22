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

#include "visitor.h"

int file_exists(const char *filename);

static void print_error(struct visitor_state *state,
                        struct code_location *startl,
                        struct code_location *endl, const char *message);

static int executable_component_exists(char *component_name,
                                       struct visitor_state *state);

static int static_component_exists(char *component_name,
                                   struct visitor_state *state);

static int component_exists(char *component_name, struct visitor_state *state);

static int component_index(char *component_name, struct visitor_state *state);

static void visit_statement(struct AST *ast, struct visitor_state *state);

static void visit_component_statement(struct component_AST *ast,
                                      struct visitor_state *state);

static void visit_test_statement(struct test_AST *ast,
                                 struct visitor_state *state);

static void visit_link_statement(struct link_AST *ast,
                                 struct visitor_state *state);

struct component *new_component(char *name, int kind, char **sources,
                                size_t sources_count) {
  struct component *component = malloc(sizeof(struct component));
  component->name = name;
  component->kind = kind;
  memcpy(component->sources, sources, sources_count * sizeof(char *));
  component->sources_count = sources_count;
  component->link_components =
      malloc(sizeof(struct component *) * LINK_COMPONENTS_BUFFER_SIZE);
  component->link_components_count = 0;
  return component;
}

struct visitor_state *new_visitor_state(const char *filename) {
  struct visitor_state *state = malloc(sizeof(struct visitor_state));
  state->filename = filename;
  state->components =
      malloc(sizeof(struct component *) * COMPONENTS_BUFFER_SIZE);
  state->components_count = 0;
  state->tests = malloc(sizeof(char *) * TESTS_BUFFER_SIZE);
  state->tests_count = 0;
  return state;
}

void visit_build_file(struct build_file_AST *ast, struct visitor_state *state) {
  for (register size_t i = 0; i < ast->asts_count; i++) {
    visit_statement(ast->asts[i], state);
  }
}

int file_exists(const char *filename) {
  FILE *file;
  if ((file = fopen(filename, "r"))) {
    fclose(file);
    return 1;
  }
  return 0;
}

static void print_error(struct visitor_state *state,
                        struct code_location *startl,
                        struct code_location *endl, const char *message) {
  println("[\"%s\" %ld:%ld-%ld:%ld]: %s", state->filename, startl->line,
          startl->column, endl->line, endl->column, message);
}

static int executable_component_exists(char *component_name,
                                       struct visitor_state *state) {
  for (register size_t i = 0; i < state->components_count; i++) {
    if (equals(component_name, state->components[i]->name) &&
        state->components[i]->kind == EXECUTABLECK)
      return 1;
  }
  return 0;
}

static int static_component_exists(char *component_name,
                                   struct visitor_state *state) {
  for (register size_t i = 0; i < state->components_count; i++) {
    if (equals(component_name, state->components[i]->name) &&
        state->components[i]->kind == STATICCK)
      return 1;
  }
  return 0;
}

static int component_index(char *component_name, struct visitor_state *state) {
  for (register size_t i = 0; i < state->components_count; i++) {
    if (equals(component_name, state->components[i]->name))
      return i;
  }
  return -1;
}

static int component_exists(char *component_name, struct visitor_state *state) {
  if (executable_component_exists(component_name, state) ||
      static_component_exists(component_name, state))
    return 1;
  return 0;
}

static void visit_statement(struct AST *ast, struct visitor_state *state) {
  switch (ast->t) {
  case COMPONENTAT:
    visit_component_statement(ast->v.component_AST, state);
    break;
  case TESTAT:
    visit_test_statement(ast->v.test_AST, state);
    break;
  case LINKAT:
    visit_link_statement(ast->v.link_AST, state);
    break;
  }
}

static void visit_component_statement(struct component_AST *ast,
                                      struct visitor_state *state) {
  for (register size_t i = 0; i < ast->sources_count; i++) {
    if (!file_exists(ast->sources[i])) {
      print_error(state, ast->sources_startls[i], ast->sources_endls[i],
                  "source file does not exist");
      exit(1);
    }
  }

  state->components[state->components_count++] =
      new_component(ast->name, ast->kind, ast->sources, ast->sources_count);
}

static void visit_test_statement(struct test_AST *ast,
                                 struct visitor_state *state) {
  if (executable_component_exists(ast->component, state)) {
    state->tests[state->tests_count++] = ast->component;
  } else {
    print_error(
        state, ast->component_startl, ast->component_endl,
        format("error: component %s does not exist or it is NOT executable",
               ast->component));
    exit(1);
  }
}

static void visit_link_statement(struct link_AST *ast,
                                 struct visitor_state *state) {
  if (component_exists(ast->main_component, state)) {
    size_t i = component_index(ast->main_component, state);
    for (register size_t j = 0; j < ast->link_components_count; j++) {
      if (static_component_exists(ast->link_components[j], state)) {
        size_t k = component_index(ast->link_components[j], state);
        state->components[i]
            ->link_components[state->components[i]->link_components_count++] =
            state->components[k];
      } else {
        print_error(
            state, ast->link_components_startls[j],
            ast->link_components_endls[j],
            format("error: component %s does not exist or it is NOT static",
                   ast->link_components[j]));
        exit(1);
      }
    }
  } else {
    print_error(state, ast->main_component_startl, ast->main_component_endl,
                format("component %s does not exist", ast->main_component));
    exit(1);
  }
}