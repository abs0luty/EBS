#include "token.h"

struct token *new_token(int type, union token_value value,
			const struct code_location *startl, const struct code_location *endl)
{
	struct token *token = malloc(sizeof(struct token));
	token->type = type;
	token->value = value;
	token->startl = startl;
	token->endl = endl;
	return token;
}

char *dump_token(const struct token *token)
{
	return format("tok(%s, [%s], %s, %s)", dump_token_type(token->type),
		      dump_token_value(token->value, token->type),
		      dump_code_location(token->startl),
		      dump_code_location(token->endl));
}

static char *dumped_token_types_table[] = {
    [STRING_TOK] = "STRING_TOK",
    [ID_TOK] = "ID_TOK",
    [COMPONENT_TOK] = "COMPONENT_TOK",
    [PUNCTUATOR_TOK] = "PUNCTUATOR_TOK",
    [EOF_TOK] = "EOF_TOK",
    [ERROR_TOK] = "ERROR_TOK",
    [STRING_NOT_CLOSED_TOK] = "STRING_NOT_CLOSED_TOK",
};

char *dump_token_type(int type)
{
	return dumped_token_types_table[type];
}

char *dump_token_value(union token_value value, int type)
{
	switch (type)
	{
	case STRING_TOK:
	case ID_TOK:
	case COMPONENT_TOK:
	case STRING_NOT_CLOSED_TOK:
		return value.str;
	case PUNCTUATOR_TOK:
	case ERROR_TOK:
	{
		char *str = malloc(2);
		str[0] = value.chr;
		str[1] = '\0';
		return str;
	}
	default:
		return "???";
	}
}

void free_token(struct token *token)
{
	free(token);
}