#include "hack-assembler/parser.h"

#include "hack-assembler/char.h"

#include <assert.h>
#include <stdlib.h>

struct parser {
	struct str input;
	const char *current;
	enum command_type current_command_type;
	struct str current_symbol;
	struct str current_dest;
	struct str current_comp;
	struct str current_jump;
};

static bool is_alphabet(char c)
{
	if (c >= 'A' && c <= 'Z') {
		return true;
	}
	else if (c >= 'a' && c <= 'z') {
		return true;
	}
	else {
		return false;
	}
}

static bool is_valid(char c)
{
	return is_alphabet(c) || char_is_digit(c)
	       || c == '_' || c == '.' || c == '$' || c == ':';
}

static bool is_whitespace(char c)
{
	return c == '\t' || c == '\n' || c == '\r' || c == ' ';
}

static void advance_to_start_of_next_command(struct parser *parser)
{
	while (parser->current != str_end(parser->input)) {
		char current_char = *parser->current;
		if (current_char == '@') {
			return;
		}
		else if (current_char == '(') {
			return;
		}
		else if (is_valid(current_char)) {
			return;
		}
		else if (current_char == '/') {
			++parser->current;
			assert(parser->current != str_end(parser->input));
			current_char = *parser->current;
			assert(current_char == '/');
			++parser->current;
			while (parser->current != str_end(parser->input)) {
				current_char = *parser->current;
				++parser->current;
				if (current_char == '\n') {
					break;
				}
			}
		}
		else {
			assert(is_whitespace(current_char));
			++parser->current;
		}
	}
}

static void advance_a_command(struct parser *parser)
{
	parser->current_command_type = A_COMMAND;

	++parser->current;
	assert(parser->current != str_end(parser->input));

	const char *start = parser->current;
	const char start_char = *start;

	if (char_is_digit(start_char)) {
		while (parser->current != str_end(parser->input)
		       && char_is_digit(*parser->current)) {
			++parser->current;
		}
	}
	else {
		assert(is_valid(start_char));
		while (parser->current != str_end(parser->input)
		       && is_valid(*parser->current)) {
			++parser->current;
		}
	}

	const char *end = parser->current; 
	assert(start != end);

	parser->current_symbol.data = start;
	parser->current_symbol.size = end - start;
}

static void advance_l_command(struct parser *parser)
{
	parser->current_command_type = L_COMMAND;

	++parser->current;
	assert(parser->current != str_end(parser->input));

	const char *start = parser->current;
	const char start_char = *start;

	assert(!char_is_digit(start_char));
	while (parser->current != str_end(parser->input)
	       && is_valid(*parser->current)) {
		++parser->current;
	}

	const char *end = parser->current; 
	assert(start != end);

	parser->current_symbol.data = start;
	parser->current_symbol.size = end - start;

	assert(parser->current != str_end(parser->input));
	assert(*parser->current == ')');
	++parser->current;
}

static void advance_c_command_comp(struct parser *parser)
{
	while (parser->current != str_end(parser->input)) {
		bool valid_char = false;
		const char current_char = *parser->current;
		if      (current_char == '0') { valid_char = true; }
		else if (current_char == '1') { valid_char = true; }
		else if (current_char == '-') { valid_char = true; }
		else if (current_char == '!') { valid_char = true; }
		else if (current_char == '+') { valid_char = true; }
		else if (current_char == '&') { valid_char = true; }
		else if (current_char == '|') { valid_char = true; }
		else if (current_char == 'A') { valid_char = true; }
		else if (current_char == 'M') { valid_char = true; }
		else if (current_char == 'D') { valid_char = true; }

		if (valid_char) {
			++parser->current;
		}
		else {
			break;
		}
	}
}

static void advance_c_command(struct parser *parser)
{
	parser->current_command_type = C_COMMAND;

	const char *start = parser->current;
	advance_c_command_comp(parser);
	const char *end = parser->current;
	assert(start != end);

	bool next_char_is_equals = false;
	if (end != str_end(parser->input)) {
		const char current_char = *parser->current;
		if (current_char == '=') {
			next_char_is_equals = true;
		}
	}

	if (next_char_is_equals) {
		parser->current_dest.data = start;
		parser->current_dest.size = end - start;

		++parser->current;

		start = parser->current;
		advance_c_command_comp(parser);
		end = parser->current;
		assert(start != end);

		parser->current_comp.data = start;
		parser->current_comp.size = end - start;
	}
	else {
		parser->current_dest.data = NULL;
		parser->current_dest.size = 0;

		parser->current_comp.data = start;
		parser->current_comp.size = end - start;
	}

	bool next_char_is_semicolon = false;
	if (end != str_end(parser->input)) {
		const char current_char = *parser->current;
		if (current_char == ';') {
			next_char_is_semicolon = true;
		}
	}

	if (next_char_is_semicolon) {
		++parser->current;

		start = parser->current;
		parser->current += 3;
		end = parser->current;
		assert(end <= str_end(parser->input));

		parser->current_jump.data = start;
		parser->current_jump.size = end - start;
	}
	else {
		parser->current_jump.data = NULL;
		parser->current_jump.size = 0;
	}
}

struct parser *parser_create(struct str input)
{
	struct parser *parser = calloc(1, sizeof(struct parser));
	assert(parser != NULL);
	parser->input = input;
	parser->current = input.data;
	advance_to_start_of_next_command(parser);
	return parser;
}

bool parser_has_more_commands(struct parser *parser)
{
	return parser->current != str_end(parser->input);
}

void parser_advance(struct parser *parser)
{
	const char *start = parser->current;
	const char start_char = *start;
	if (start_char == '@') {
		advance_a_command(parser);
	}
	else if (is_valid(start_char)) {
		advance_c_command(parser);
	}
	else if (start_char == '(') {
		advance_l_command(parser);
	}

	assert(parser->current != start);
	advance_to_start_of_next_command(parser);
}

enum command_type parser_command_type(struct parser *parser)
{
	return parser->current_command_type;
}

struct str parser_symbol(struct parser *parser)
{
	return parser->current_symbol;
}

struct str parser_dest(struct parser *parser)
{
	return parser->current_dest;
}

struct str parser_comp(struct parser *parser)
{
	return parser->current_comp;
}

struct str parser_jump(struct parser *parser)
{
	return parser->current_jump;
}
