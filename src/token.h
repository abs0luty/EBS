#ifndef _token_h_
#define _token_h_

#include "code_location.h"
#include "str.h"

union token_value {
	char* str;
	char chr;
};

#define TVALUE(key, value) (union token_value) {.key = value}

struct token {
	enum {
		STRING_TOK,
		ID_TOK,
		COMPONENT_TOK,
		PUNCTUATOR_TOK,
		EOF_TOK,
		ERROR_TOK,
		STRING_NOT_CLOSED_TOK,
	} type;
	union token_value value;
	const struct code_location* startl, *endl;
};

struct token* new_token(int type, union token_value value, 
		const struct code_location* startl, const struct code_location* endl);

char* dump_token(const struct token* token);

char* dump_token_type(int type);

char* dump_token_value(union token_value value, int type);

void free_token(struct token* token);

#endif /* _token_h_ */
