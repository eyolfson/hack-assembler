#include "hack-assembler/symbol_table.h"

#include <assert.h>
#include <stdlib.h>

int main()
{
	struct symbol_table *symbol_table = symbol_table_create();
	assert(symbol_table != NULL);

	struct str i = str_from_c_str("i");

	symbol_table_add_entry(symbol_table, i, 0xBEEF);
	assert(symbol_table_contains(symbol_table, i));
	assert(symbol_table_get_address(symbol_table, i) == 0xBEEF);

	struct str j = str_from_c_str("j");
	assert(!symbol_table_contains(symbol_table, j));

	free(symbol_table);

	return 0;
}
