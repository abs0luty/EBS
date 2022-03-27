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

#include "compile.h"

static void compile_component(const struct component *component);

static void compile_components(const struct visitor_state *state);

static void test_component(const char *name);

void compile(const struct visitor_state *state) {
#ifdef useGCC
#define compiler "gcc"
#elif useCLANG
#define compiler "clang"
#elif useCC
#define compiler "cc"
#else
#error "no compiler specified: use -useGCC, -useCLANG or -useCC"
#define compiler "unknown"
#endif

  println("[INFO]: compiler: %s", compiler);

  if (state->tests_count == 0)
    println("[INFO]: no tests to run through");

  compile_components(state);

  for (size_t i = 0; i < state->tests_count; ++i) {
    println("[INFO]: testing \"%s\"", state->tests[i]);
    test_component(state->tests[i]);
    println("[SUCESS]: test \"%s\" passed", state->tests[i]);
  }
}

static void compile_component(const struct component *component) {
  for (register size_t i = 0; i < component->sources_count; i++) {
    char *source = component->sources[i];
    char *command =
        format("%s -c %s -o singleObject_%s.o", compiler, source, source);
    int exit_code = system(command);
    if (exit_code != 0) {
      println("[ERROR]: failed to compile single object: \"%s\"", source);
      exit(1);
    }

    println("[SUCCESS]: generated single object for \"%s\"", source);
  }

  char *link_command = format("ld -relocatable -o %s.o ", component->name);

  for (register size_t i = 0; i < component->sources_count; i++) {
    link_command =
        format("%s singleObject_%s.o ", link_command, component->sources[i]);
    println("[INFO]: single object %s will be linked with the component",
            component->sources[i]);
  }

  for (register size_t i = 0; i < component->link_components_count; i++) {
    link_command =
        format("%s %s.o ", link_command, component->link_components[i]->name);
    println("[INFO]: object %s will be linked with the component",
            component->link_components[i]->name);
  }

  int exit_code = system(link_command);
  if (exit_code != 0) {
    println("[ERROR]: failed to link component \"%s\"", component->name);
    exit(1);
  }

  println("[INFO]: linked \"%s\" (%s.o)", component->name, component->name);

  for (register size_t i = 0; i < component->sources_count; i++) {
    remove(format("singleObject_%s.o", component->sources[i]));
    println("[INFO]: removed single object for \"%s\" (singleObject_%s.o)",
            component->sources[i], component->sources[i]);
  }

  if (component->kind == EXECUTABLECK) {
    println("[INFO]: linking executable component: \"%s\"", component->name);
    exit_code = system(
        format("%s %s.o -o %s", compiler, component->name, component->name));
    if (exit_code != 0) {
      println("[ERROR]: failed to link executable component \"%s\"",
              component->name);
      exit(1);
    }

    remove(format("%s.o", component->name));
    println("[INFO]: removed executable object for \"%s\" (%s.o)",
            component->name, component->name);
  }
}

static void link_components(const struct component *main_component,
                            const struct component **link_components,
                            size_t link_components_count) {

  char *link_command = format("ld -relocatable -o %s.o ", main_component->name);

  for (register size_t i = 0; i < link_components_count; i++) {
    link_command = format("%s %s.o ", link_command, link_components[i]->name);
  }

  system(link_command);
  println("[INFO]: relinked \"%s\" (%s.o)", main_component->name,
          main_component->name);
}

static void compile_components(const struct visitor_state *state) {
  for (size_t i = 0; i < state->components_count; ++i) {
    println("[INFO]: compiling component: %s", state->components[i]->name);
    compile_component(state->components[i]);
  }
}

static void test_component(const char *name) {
  int exit_code = system(format("./%s", name));
  if (exit_code != 0) {
    println("[FAIL]: testing the component %s failed with exit code %d\n", name,
            exit_code);
    exit(1);
  }
}
