#pragma once

#include "hack-assembler/command_type.h"
#include "hack-assembler/str.h"

#include <stdbool.h>

struct parser;

struct parser *parser_create(struct str input);
bool parser_has_more_commands(struct parser *parser);
void parser_advance(struct parser *parser);
enum command_type parser_command_type(struct parser *parser);
struct str parser_symbol(struct parser *parser);
struct str parser_dest(struct parser *parser);
struct str parser_comp(struct parser *parser);
struct str parser_jump(struct parser *parser);
