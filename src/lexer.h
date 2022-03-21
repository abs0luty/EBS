#ifndef _lexer_h_
#define _lexer_h_

#include <stdlib.h>
#include <ctype.h>
#include "token.h"
#include "code_location.h"

struct lexer_state {
    const char* filename, *input;
    struct code_location* location;
};

struct lexer_state* new_lexer_state(const char* filename, const char* input);

struct token* next_token(struct lexer_state* state);

void free_lexer_state(struct lexer_state* state);

#endif /* _lexer_h_ */