#include "lexer.h"
#include "file.h"
#include "println.h"
#include <time.h>

int main(size_t argc, const char **argv)
{
    const char *filename = argv[1];

    if (!filename)
    {
        println("usage: %s <filename>", argv[0]);
        return 1;
    }

    char *input = read_file(filename);

    if (!input)
    {
        println("could not read file %s", filename);
        return 1;
    }

    struct lexer_state *state = new_lexer_state(filename, input);
    struct token *token = NULL;

    clock_t start = clock();

    while (token == NULL || token->type != EOF_TOK)
    {
        token = next_token(state);
        println("%s", dump_token(token));
    }

    println("finished in %f s.", (double)(clock() - start) / CLOCKS_PER_SEC);

    free_lexer_state(state);
    free_token(token);
}