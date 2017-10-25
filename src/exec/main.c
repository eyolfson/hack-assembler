#include "hack-assembler/code.h"
#include "hack-assembler/parser.h"
#include "hack-assembler/symbol_table.h"

#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

static char *output_path_create(const char *input_path)
{
	size_t input_path_size = strlen(input_path);
	assert(strcmp(input_path + (input_path_size - 4), ".asm") == 0);

	char *output_path = malloc(input_path_size + 2);
	output_path[0] = 0;
	strncat(output_path, input_path, input_path_size - 4);
	strcat(output_path, ".hack");

	return output_path;
}

static struct str input_create(const char *input_path)
{
	int fd = open(input_path, O_RDONLY);
	assert(fd >= 0);

	struct stat stat;
	int ret = fstat(fd, &stat);
	assert(ret == 0);
	size_t size = stat.st_size;

	char *data = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
	assert(data != MAP_FAILED);
	close(fd);

	struct str input = {
		.data = data,
		.size = size
	};
	return input;
}

static void input_destroy(struct str input)
{
	int ret = munmap((char *) input.data, input.size);
	assert(ret == 0);
}

static void build_symbol_table_predefined(struct symbol_table *symbol_table)
{
	symbol_table_add_entry(symbol_table, str_from_c_str("SP"), 0);
	symbol_table_add_entry(symbol_table, str_from_c_str("LCL"), 1);
	symbol_table_add_entry(symbol_table, str_from_c_str("ARG"), 2);
	symbol_table_add_entry(symbol_table, str_from_c_str("THIS"), 3);
	symbol_table_add_entry(symbol_table, str_from_c_str("THAT"), 4);
	symbol_table_add_entry(symbol_table, str_from_c_str("R0"), 0);
	symbol_table_add_entry(symbol_table, str_from_c_str("R1"), 1);
	symbol_table_add_entry(symbol_table, str_from_c_str("R2"), 2);
	symbol_table_add_entry(symbol_table, str_from_c_str("R3"), 3);
	symbol_table_add_entry(symbol_table, str_from_c_str("R4"), 4);
	symbol_table_add_entry(symbol_table, str_from_c_str("R5"), 5);
	symbol_table_add_entry(symbol_table, str_from_c_str("R6"), 6);
	symbol_table_add_entry(symbol_table, str_from_c_str("R7"), 7);
	symbol_table_add_entry(symbol_table, str_from_c_str("R8"), 8);
	symbol_table_add_entry(symbol_table, str_from_c_str("R9"), 9);
	symbol_table_add_entry(symbol_table, str_from_c_str("R10"), 10);
	symbol_table_add_entry(symbol_table, str_from_c_str("R11"), 11);
	symbol_table_add_entry(symbol_table, str_from_c_str("R12"), 12);
	symbol_table_add_entry(symbol_table, str_from_c_str("R13"), 13);
	symbol_table_add_entry(symbol_table, str_from_c_str("R14"), 14);
	symbol_table_add_entry(symbol_table, str_from_c_str("R15"), 15);
	symbol_table_add_entry(symbol_table, str_from_c_str("SCREEN"), 16384);
	symbol_table_add_entry(symbol_table, str_from_c_str("KBD"), 24576);
}

static void build_symbol_table_labels(struct str input,
                                      struct symbol_table *symbol_table)
{
	struct parser *parser = parser_create(input);
	uint16_t next_address = 0;
	while (parser_has_more_commands(parser)) {
		parser_advance(parser);

		if (parser_command_type(parser) == L_COMMAND) {
			struct str symbol = parser_symbol(parser);
			symbol_table_add_entry(symbol_table, symbol, next_address);
		}
		else {
			++next_address;
		}
	}
	free(parser);
}

static void build_symbol_table_variables(struct str input,
                                         struct symbol_table *symbol_table)
{
	struct parser *parser = parser_create(input);
	uint16_t next_address = 16;
	while (parser_has_more_commands(parser)) {
		parser_advance(parser);

		if (parser_command_type(parser) == A_COMMAND) {
			struct str symbol = parser_symbol(parser);
			if (!str_starts_with_digit(symbol)
			    && !symbol_table_contains(symbol_table, symbol)) {
				symbol_table_add_entry(symbol_table, symbol, next_address);
				++next_address;
			}
		}
	}
	free(parser);
}

static void build_symbol_table(struct str input,
                               struct symbol_table *symbol_table)
{
	build_symbol_table_predefined(symbol_table);
	build_symbol_table_labels(input, symbol_table);
	build_symbol_table_variables(input, symbol_table);
}

static void output_command(struct parser *parser,
                           struct symbol_table *symbol_table,
                           int fd)
{
	char line[17];
	line[16] = '\n';

	uint16_t code = 0;
	enum command_type command_type = parser_command_type(parser);
	if (command_type == A_COMMAND) {
		struct str symbol = parser_symbol(parser);
		if (str_starts_with_digit(symbol)) {
			code = str_to_uint15_t(symbol);
		}
		else {
			code = symbol_table_get_address(symbol_table, symbol);
		}
	}
	else if (command_type == C_COMMAND) {
		struct str dest = parser_dest(parser);
		struct str comp = parser_comp(parser);
		struct str jump = parser_jump(parser);

		code = 0b1110000000000000;
		code |= code_comp(comp) << 6;
		if (!str_is_null(dest)) {
			code |= code_dest(dest) << 3;
		}
		if (!str_is_null(jump)) {
			code |= code_jump(jump);
		}
	}
	else if (command_type == L_COMMAND) {
		return;
	}

	for (size_t i = 0; i < 16; ++i) {
		if (code & (1 << (15 - i))) {
			line[i] = '1';
		}
		else {
			line[i] = '0';
		}
	}

	ssize_t bytes_written = write(fd, line, sizeof(line));
	assert(bytes_written == sizeof(line));
}

static void output_commands(struct str input,
                            struct symbol_table *symbol_table,
                            int fd)
{
	struct parser *parser = parser_create(input);
	while (parser_has_more_commands(parser)) {
		parser_advance(parser);
		output_command(parser, symbol_table, fd);
	}
	free(parser);
}

int main(int argc, const char *const *argv)
{
	assert(argc == 2);
	const char *input_path = argv[1];

	struct str input = input_create(input_path);
	struct symbol_table *symbol_table = symbol_table_create();
	build_symbol_table(input, symbol_table);

	char *output_path = output_path_create(input_path);
	mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	int fd = open(output_path, O_WRONLY | O_CREAT | O_TRUNC, mode);
	assert(fd >= 0);
	output_commands(input, symbol_table, fd);
	close(fd);

	free(symbol_table);
	input_destroy(input);

	free(output_path);

	return 0;
}
