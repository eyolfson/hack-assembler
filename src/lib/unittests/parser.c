#include "hack-assembler/parser.h"

#include <assert.h>
#include <stdlib.h>

static void test_whitespace()
{
	struct str input = str_from_c_str("\t\r\n \t\r\n");

	struct parser *parser = parser_create(input);
	assert(parser != NULL);

	assert(!parser_has_more_commands(parser));

	free(parser);
}

static void test_simple()
{
	struct str input = str_from_c_str("@2\nA\nD=A\nD=A;JEQ");

	struct parser *parser = parser_create(input);
	assert(parser != NULL);

	assert(parser_has_more_commands(parser));
	parser_advance(parser);
	assert(parser_command_type(parser) == A_COMMAND);
	assert(str_equal(parser_symbol(parser), str_from_c_str("2")));

	assert(parser_has_more_commands(parser));
	parser_advance(parser);
	assert(parser_command_type(parser) == C_COMMAND);
	assert(str_is_null(parser_dest(parser)));
	assert(str_equal(parser_comp(parser), str_from_c_str("A")));
	assert(str_is_null(parser_jump(parser)));

	assert(parser_has_more_commands(parser));
	parser_advance(parser);
	assert(parser_command_type(parser) == C_COMMAND);
	assert(str_equal(parser_dest(parser), str_from_c_str("D")));
	assert(str_equal(parser_comp(parser), str_from_c_str("A")));
	assert(str_is_null(parser_jump(parser)));

	assert(parser_has_more_commands(parser));
	parser_advance(parser);
	assert(parser_command_type(parser) == C_COMMAND);
	assert(str_equal(parser_dest(parser), str_from_c_str("D")));
	assert(str_equal(parser_comp(parser), str_from_c_str("A")));
	assert(str_equal(parser_jump(parser), str_from_c_str("JEQ")));

	assert(!parser_has_more_commands(parser));

	free(parser);
}

int main()
{
	test_whitespace();
	test_simple();
	return 0;
}
