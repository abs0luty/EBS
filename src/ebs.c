#include "lexer.h"

int main() {
    struct lexer_state* state = new_lexer_state("test.c", "int main() { return 0; }");
    struct token* token = next_token(state);

    printf("%d", token->type);
}