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

#ifndef _visitor_h_
#define _visitor_h_

#include "AST.h"
#include "println.h"
#include "str.h"
#include <stdlib.h>

#define LINK_COMPONENTS_BUFFER_SIZE 1024

struct component {
  char *name;
  int kind;

  char **sources;
  size_t sources_count;

  struct component **link_components;
  size_t link_components_count;
};

struct component *new_component(char *name, int kind, char **sources,
                                size_t sources_count);

#define COMPONENTS_BUFFER_SIZE 10000
#define TESTS_BUFFER_SIZE 100000

struct visitor_state {
  const char *filename;

  struct component **components;
  size_t components_count;

  char **tests;
  size_t tests_count;
};

struct visitor_state *new_visitor_state(const char *filename);

void visit_build_file(struct build_file_AST *ast, struct visitor_state *state);

#endif /* _visitor_h_ */