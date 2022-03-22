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

#include "AST.h"

struct build_file_AST* new_build_file_AST(struct AST** asts,
    size_t asts_count) {
    struct build_file_AST* build_file_AST =
        malloc(sizeof(struct build_file_AST));
    build_file_AST->asts = asts;
    build_file_AST->asts_count = asts_count;
    return build_file_AST;
}

struct AST* AST_from_component_AST(struct component_AST* component_AST) {
    struct AST* AST = malloc(sizeof(struct AST));
    AST->t = COMPONENTAT;
    AST->v.component_AST = component_AST;
    return AST;
}

struct AST* AST_from_link_AST(struct link_AST* link_AST) {
    struct AST* AST = malloc(sizeof(struct AST));
    AST->t = LINKAT;
    AST->v.link_AST = link_AST;
    return AST;
}

struct AST* AST_from_test_AST(struct test_AST* test_AST) {
    struct AST* AST = malloc(sizeof(struct AST));
    AST->t = TESTAT;
    AST->v.test_AST = test_AST;
    return AST;
}

struct component_AST* new_component_AST(
    int kind, char* name, char** sources, size_t sources_count) {
    struct component_AST* component_AST = malloc(sizeof(struct component_AST));
    component_AST->kind = kind;
    component_AST->name = name;
    component_AST->sources = sources;
    component_AST->sources_count = sources_count;
}

struct link_AST* new_link_AST(
    char* main_component, char** link_components,
    size_t link_components_count) {
    struct link_AST* link_AST = malloc(sizeof(struct link_AST));
    link_AST->main_component = main_component;
    link_AST->link_components = link_components;
    link_AST->link_components_count = link_components_count;
}

struct test_AST* new_test_AST(char* component) {
    struct test_AST* test_AST = malloc(sizeof(struct test_AST));
    test_AST->component = component;
    return test_AST;
}