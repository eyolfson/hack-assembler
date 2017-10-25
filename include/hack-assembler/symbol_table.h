#pragma once

#include "hack-assembler/str.h"

#include <stdbool.h>
#include <stdint.h>

struct symbol_table;
struct symbol_table *symbol_table_create();
void symbol_table_add_entry(struct symbol_table *symbol_table,
                            struct str symbol, uint16_t address);
bool symbol_table_contains(struct symbol_table *symbol_table,
                           struct str symbol);
uint16_t symbol_table_get_address(struct symbol_table *symbol_table,
                                  struct str symbol);
