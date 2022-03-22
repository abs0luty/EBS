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